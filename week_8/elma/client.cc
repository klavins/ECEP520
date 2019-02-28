#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"
#include "elma.h"
#include <thread>
#include <tuple>
#include <iostream>

namespace elma {

    std::pair<std::string,std::string> Client::url_parts(std::string url) {
        
        std::string protocol = url.substr(0,url.find("://"));
        if ( protocol == "http" ) {
            _use_ssl = false;
        } else if ( protocol == "https" ) {
            _use_ssl = true;
        } else {
            throw Exception("Protocol " + protocol + " not implemented in Client.");
        } 

        std::string rest = url.substr(protocol.length() + 3),
                    addr = rest.substr(0, rest.find("/")),
                    path = rest.substr(addr.length());

        if ( path == "" ) {
            path = "/";
        }

        return std::make_pair(addr,path);

    }

    Client& Client::get(std::string url, std::function<void(json&)> handler) {
        std::thread t(&Client::_get_thread,this,url,handler);
        t.detach(); // detaching means we don't have to join later
    }

    Client& Client::process_responses() {

        _mtx.lock();

        for(auto response : _responses ) {
            std::get<1>(response)(std::get<0>(response));
        }

        _responses.clear();

        _mtx.unlock();

        return *this;

    }

    const std::shared_ptr<httplib::Response> Client::_get_aux(std::string url) {
        auto parts = url_parts(url);
        if ( _use_ssl ) {
            httplib::SSLClient cli(parts.first.c_str(), 443);
            return cli.Get(parts.second.c_str());              
        } else {
            httplib::Client cli(parts.first.c_str(), 80);
            return cli.Get(parts.second.c_str());              
        }        
    }

    void Client::_get_thread(std::string url, std::function<void(json&)> handler) {

        json json_response;

        try {

            auto response = _get_aux(url);

            if (response && response->status == 200) {
                json_response = json::parse(response->body);
             } else if ( response ) {
                std::cout << "Warning:: Elma client connected to a server that returned Error: " 
                          << response->status 
                          << std::endl;
            } else {
                std::cout << "Warning:: Elma client returned no result" 
                          << std::endl;
            }

        } catch (const httplib::Exception& e) {
            std::cout << "Warning: Elma client failed: " 
                      << e.what() 
                      << "\n";
        } catch(const json::exception& e ) {
            std::cout << "Warning: Elma client could not parse response: " 
                      << e.what() 
                      << "\n";
        } catch (...) {
            std::cout << "Warning: Elma client failed with no message\n";
        }

        _mtx.lock();
        _responses.push_back(std::make_tuple(json_response, handler));
        _mtx.unlock();

    }    

};