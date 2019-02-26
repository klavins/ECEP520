#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace elma {

    //! An exception class for Elma.

    //! Exception messages will be prefixed with "Elma error: "
    //! but otherwise look like run time errors.
    class Exception : public std::runtime_error {

        public:
        Exception(const std::string& what) : 
          runtime_error(what), 
          _what("Elma Error: " + what) {}

        const char* what() const throw (){
            return _what.c_str();
        }

        private:
        std::string _what;

    };

}

#endif