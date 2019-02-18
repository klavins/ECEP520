#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace elma {

    class Exception : public std::runtime_error {

        public:
        Exception(const std::string& what) : runtime_error(what), _what(what) {}

        const char* what() const throw (){
            std::string msg = "ELMA Exception: " + _what;
            return msg.c_str();
        }

        private:
        std::string _what;

    };

}

#endif