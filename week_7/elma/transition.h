#ifndef _TRANSITION_H
#define _TRANSITION_H

#include <string>
#include "elma.h"

namespace elma {

    class State;

    //! States for finite state machines (FSM)
    class Transition {

        public:
        Transition(std::string event_name, State& from, State& to) : 
          _from(from),
          _to(to),
          _event_name(event_name) {}

        private:
        State& _from;
        State& _to;
        string _event_name;

    };

}

#endif