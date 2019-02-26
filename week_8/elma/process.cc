#include <stdexcept>
#include "elma.h"

namespace elma {

    //! Access a channel with the given name
    /*!
      \param name The name of the channel
      \return A reference to the channel
    */
    Channel& Process::channel(string name) {
        if ( _manager_ptr == NULL ) {
            throw Exception("Cannot access channels in a process before the process is scheduled.");
        } else {
            return _manager_ptr->channel(name);
        }
    }

    void Process::watch(string event_name, std::function<void(Event&)> handler) {
        if ( _manager_ptr == NULL ) {
            throw Exception("Cannot access events in a process before the process is scheduled.");
        } else {        
            _manager_ptr->watch(event_name, handler);
        }
    }

    void Process::emit(const Event& event) {
        if ( _manager_ptr == NULL ) {
            throw Exception("Cannot access events in a process before the process is scheduled.");
        } else {        
            _manager_ptr->emit(event);
        }
    }

    void Process::http_get(std::string url, std::function<void(json&)> handler) {
        _manager_ptr->client().get(url,handler);
    }

    //! The time since the last update in millisconds, as a double
    /*!
      \return The time since the last update, in milliseconds
    */
    double Process::milli_time() {
        duration<double, std::milli> time = last_update();
        return time.count();
    }

    //! The most recent amount of time between updates.
    /*!
      A common use of this method is in the update() method for Euler integration
      as in xnew = x + delta() * f(x)
      \return The most recent delta, in milliseconds
    */    
    double Process::delta() { 
        duration<double, std::milli> diff = last_update() - previous_update();
        return diff.count();
    }

    // Manager interface for the _init method. Do not call directly. 
    void Process::_init() { 
        _status = STOPPED;     
        init();
    }
    // Manager interface for the _start method. Do not call directly. 
    void Process::_start(high_resolution_clock::duration elapsed) { 
        _status = RUNNING; 
        _start_time = high_resolution_clock::now();
        _last_update = elapsed;
        _num_updates = 0;
        start();
    }

    // Manager interface for the _update method. Do not call directly. 
    void Process::_update(high_resolution_clock::duration elapsed) {
        _previous_update = _last_update;
        _last_update = elapsed;
        update();
        _num_updates++;
    }

    // Manager interface for the _stop method. Do not call directly. 
    void Process::_stop() { 
        _status = STOPPED; 
        stop();
    }       

}