#include <stdexcept>
#include "elma.h"

namespace elma {

    //! Access a channel with the given name
    /*!
      A more elaborate description of the constructor.
      \param name The name of the channel
      \return A reference to the process, for method chaining
    */
    Channel& Process::channel(string name) {
        return _manager_ptr->channel(name);
    }

    double Process::milli_time() {
        duration<double, std::milli> time = last_update();
        return time.count();
    }
    double Process::delta() { 
        duration<double, std::milli> diff = last_update() - previous_update();
        return diff.count();
    }

    // Manager interface
    void Process::_init() { 
        _status = STOPPED;     
        init();
    }
    void Process::_start(high_resolution_clock::duration elapsed) { 
        _status = RUNNING; 
        _start_time = high_resolution_clock::now();
        _last_update = elapsed;
        _num_updates = 0;
        start();
    }
    void Process::_update(high_resolution_clock::duration elapsed) {
        _previous_update = _last_update;
        _last_update = elapsed;
        update();
        _num_updates++;
    }
    void Process::_stop() { 
        _status = STOPPED; 
        stop();
    }       

}