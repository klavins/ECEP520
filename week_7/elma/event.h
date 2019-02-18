#ifndef _EVENT_H
#define _EVENT_H

#include <string>
#include <vector>
#include <json/json.h>

using std::string;
using std::vector;
using nlohmann::json; 

namespace elma {

    //! Events
    class Event {

        public:
        Event(json value) : _value(value) {}
        inline json value() const { return _value; }

        private:
        json _value;

    };

}

#endif