#ifndef _CLIENT_H
#define _CLIENT_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "/source/httplib/httplib.h"
#include <string>
#include <tuple>
#include "elma.h"

namespace elma {

    class Client {

        public:
        Client() : _use_ssl(false) {}
        Client& get(std::string url, std::function<void(json&)> handler);
        Client& process_responses();
        Client& use_ssl(bool val) { _use_ssl = false; }

        //! Parse a URL into its address and path
        //! \param url The url
        //! \return A std::pair containing the parts
        std::pair<std::string,std::string> url_parts(std::string url);

        //! \return The number of responses
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