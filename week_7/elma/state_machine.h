#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "elma.h"

namespace elma {

    //! States for finite state machines (FSM)
    class StateMachine : public Process {

        public:
        StateMachine(std::string name) : Process(name) {}

        StateMachine& set_initial(State& s);
        StateMachine& add_transition(std::string event_name, State& from, State& to);

        void init();
        void start();
        void update();
        void stop();

        private:
        vector<Transition> _transitions;
        State * _initial;
        State * _current;

    };

}

#endif