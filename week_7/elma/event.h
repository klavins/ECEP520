#ifndef _EVENT_H
#define _EVENT_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace elma {

    //! Events
    class Event {

        public:
        Event(string sval) : _string_value(sval), _vector_value(1,0) {}
        Event(string sval, vector<double> vval) : _string_value(sval), _vector_value(vval) {}
        inline vector<double> vector_value() { return _vector_value; }
        inline string string_value() { return _string_value; }

        private:
        string _string_value;
        vector<double> _vector_value;

    };

}

#endif