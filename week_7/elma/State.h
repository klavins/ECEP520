#ifndef _STATE_H
#define _STATE_H

#include <string>
#include "elma.h"

namespace elma {

    class StateMachine;

    //! States for finite state machines (FSM)
    class State {

        friend class StateMachine;

        public:
        State() : _name("unnamed state") {
            _id = _id_counter++;
        }
        State(std::string name) : _name(name){
            _id = _id_counter++;
        }
        inline std::string name() { return _name; }
        inline int id() { return _id; }
        virtual void entry(const Event& e) = 0;
        virtual void during() = 0;
        virtual void exit(const Event& e) = 0;

        void emit(const Event& e);

        private:
        std::string _name;
        int _id;
        static int _id_counter;
        StateMachine * _state_machine_ptr;

    };

}

#endif