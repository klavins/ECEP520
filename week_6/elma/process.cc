#include <stdexcept>
#include "elma.h"

namespace elma {

    Channel& Process::channel(string name) {
        return _manager_ptr->channel(name);
    }

}