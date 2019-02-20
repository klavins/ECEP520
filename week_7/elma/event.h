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
        Event(json value) : _value(value), _propagate(true) {}
        inline json value() const { return _value; }
        inline bool propagate() const { return _propagate; }
        inline void stop_propagation() { _propagate = false; }
        inline void reset() { _propagate = true; }

        private:
        json _value;
        bool _propagate;

    };

}

#endif