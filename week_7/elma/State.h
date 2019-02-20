#ifndef _STATE_H
#define _STATE_H

#include "elma.h"

namespace elma {

    //! States for finite state machines (FSM)
    class State {

        public:
        State() {}
        virtual void entry() = 0;
        virtual void during() = 0;
        virtual void exit() = 0;

    };

}

#endif