#include "elma.h"

namespace elma {

    void State::emit(const Event& e) {
        if ( _state_machine_ptr == NULL ) {
            throw Exception("Cannot access events in a state before the is added to a state machine.");
        } else {                
            _state_machine_ptr->emit(e);
        }
    }

}