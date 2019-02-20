#ifndef _STATE_H
#define _STATE_H

#include "elma.h"

namespace elma {

    //! States for finite state machines (FSM)
    class State {

        public:
        State() : _name("unamed state"){
            _id = _id_counter++;
        }
        State(std::string name) : _name(name){
            _id = _id_counter++;
        }
        inline std::string name() { return _name; }
        inline int id() { return _id; }
        virtual void entry() = 0;
        virtual void during() = 0;
        virtual void exit() = 0;

        private:
        std::string _name;
        int _id;
        static int _id_counter;

    };

}

#endif