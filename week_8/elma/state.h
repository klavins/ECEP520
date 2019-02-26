#ifndef _STATE_H
#define _STATE_H

#include <string>
#include "elma.h"

namespace elma {

    class StateMachine;

    //! States for the StateMachine class

    //! States are used with the StateMachine class. For example usage, see the documentation for that class.
    class State {

        friend class StateMachine;

        public:

        //! Construct an unnamed State
        State() : _name("unnamed state"), _state_machine_ptr(NULL) {
            _id = _id_counter++;
        }

        //! Construct a state with the given name
        State(std::string name) : _name(name), _state_machine_ptr(NULL) {
            _id = _id_counter++;
        }

        //! \return The name of the state
        inline std::string name() { return _name; }

        //! \return The id of the state (used internally, do not depend on its result)
        inline int id() { return _id; }

        //! A method that derived instances should define. It is called when the state is
        //! entered by the state machine either when the machine starts or when a transition
        //! to the state is fired.
        //! \param e The event that led to the transition into the state
        virtual void entry(const Event& e) = 0;

        //! A method that derived instances should define. It is called repeatedly by the
        //! update() method of the containing StateMachine when the state is active.
        virtual void during() = 0;

        //! A method that derived instances should define. It is called just before the state is
        //! exited by the state machine when a transition
        //! from the state is fired.
        //! \param e The event that led to the transition out of the state        
        virtual void exit(const Event& e) = 0;

        //! Emit an event. A wrapper around the emit() method of Manager
        //! \param e The event to emit
        void emit(const Event& e);

        private:
        std::string _name;
        int _id;
        static int _id_counter;
        StateMachine * _state_machine_ptr;

    };

}

#endif