#include "elma.h"

namespace elma {

    void State::emit(const Event& e) {
        _state_machine_ptr->emit(e);
    }

}