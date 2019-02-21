#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "elma.h"

namespace elma {

    //! States for finite state machines (FSM)
    class StateMachine : public Process {

        public:
        StateMachine(std::string name) : Process(name) {}
        StateMachine() : Process("unnamed state machine") {}

        StateMachine& set_initial(State& s);
        StateMachine& add_transition(std::string event_name, State& from, State& to);
        inline StateMachine& set_propagate(bool val) { _propagate = val; }

        State& current() { return *_current; }

        void init();
        void start();
        void update();
        void stop();

        private:
        vector<Transition> _transitions;
        State * _initial;
        State * _current;
        bool _propagate;

    };

}

#endif