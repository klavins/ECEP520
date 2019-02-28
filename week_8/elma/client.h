#ifndef _CLIENT_H
#define _CLIENT_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"

#include <string>
#include <tuple>
#include "elma.h"

namespace elma {

    //! An HTTP client for connecting to json services

    //! An example usage in a process
    //! @code
    //! class GetTester : public Process {
    //!     public:
    //!     GetTester() : Process("Get Tester") {}
    //!     void init() {}
    //!     void start() {
    //!         got_response = false;
    //!         http_get("https://api.github.com/repos/klavins/ecep520", [this](json& response) {
    //!             got_response = true;
    //!         });
    //!     }
    //!     void update() {}
    //!     void stop() {
    //!         ASSERT_EQ(true,got_response);
    //!     }
    //!     bool got_response;
    //! };
    //! @endcode    
    class Client {

        public:

        //! Construct a new client. Only the Manager would normall do this, although
        //! a client can work as a standalone object.
        Client() : _use_ssl(false) {}

        //! Send an HTTP GET request to a specific URL and register a handler 
        //! to deal with the response. This method assumes the server will respond
        //! with a JSON string. This method is asynchronous and returns immediately.
        //! \param url The url, preceded by http:// or https://
        //! \param handler The handler, whose argument will be the json received from the request
        //! \return A reference to the client, for chaining
        Client& get(std::string url, std::function<void(json&)> handler);

        //! Process all responses received so far
        //! \return A reference to the client, for chaining        
        Client& process_responses();

        //! Parse a URL into its address and path
        //! \param url The url
        //! \return A std::pair containing the parts
        std::pair<std::string,std::string> url_parts(std::string url);

        //! \return The number of unprocessed responses
        int num_responses() const { return _responses.size(); }

        private:

        void _get_thread(std::string url, std::function<void(json&)> handler);
        const std::shared_ptr<httplib::Response> _get_aux(std::string url);
        std::vector<std::tuple<json, std::function<void(json&)>>> _responses;
        bool _use_ssl;
        std::mutex _mtx;

    };

}

#endif