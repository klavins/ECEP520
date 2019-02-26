#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "elma.h"

namespace elma {

    //! A finite state machine class

    //! Together with the State and Transition classes, this class is used to make a finite state machine process.
    //! For example, here is a toggle switch machine
    //! \include examples/toggle-switch.cc
    class StateMachine : public Process { 

        public:

        //! Construct a new StateMachine with the given name
        StateMachine(std::string name) : Process(name), _initial(NULL), _current(NULL), _propagate(false) {}

        //! Construct an unnamed StateMachine       
        StateMachine() : Process("unnamed state machine"), _initial(NULL), _current(NULL), _propagate(false) {}

        //! Set the initial state of the state machine
        //! \param s An instantiation of a class derived from State
        //! \return A reference to the state machine, for chaining         
        StateMachine& set_initial(State& s);

        //! Add a transition to the state machine
        //! \param event_name Events with this name will trigger the transition
        //! \param from The state the machine must be in to take the transition
        //! \param to The state the machine will go to upon taking the transition 
        //! \return A reference to the state machine, for chaining          
        StateMachine& add_transition(std::string event_name, State& from, State& to);

        //! Set whether the state machine should propagate transitions (default = false)
        //! \param val True or false 
        //! \return A reference to the state machine, for chaining    
        inline StateMachine& set_propagate(bool val) { _propagate = val; }

        //! \return The current state
        State& current() { return *_current; }

        //! Do not override init() for a state machine.
        void init();

        //! Do not override init() for a state machine.        
        void start();

        //! Do not override init() for a state machine.        
        void update();

        //! Do not override init() for a state machine.        
        void stop();

        private:
        vector<Transition> _transitions;
        State * _initial;
        State * _current;
        bool _propagate;

    };

}

#endif