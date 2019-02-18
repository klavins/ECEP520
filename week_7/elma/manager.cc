#include <stdexcept>
#include <iostream>
#include "elma.h"

namespace elma {

    //! Add a Process to the manager, to be run at a certain frequency.
    //! \param process The process to be scheduled, usually derived from the Process abstract base class
    //! \param period The desired duration of time between updates
    //! \return A reference to the manager, for chaining
    Manager& Manager::schedule(
        Process& process, 
        high_resolution_clock::duration period) {

        process._period = period;
        _processes.push_back(&process); 
        process._manager_ptr = this;            

        return *this;

    }

    //! Add a channel to the manager
    //! \param The channel to be added
    //! \return A reference to the manager, for chaining
    Manager& Manager::add_channel(Channel& channel) {
        _channels[channel.name()] = &channel;
        return *this;
    }

    //! Retrieve a reference to an existing channel. Throws an error if no such channel exists.
    //! \return The channel requested.
    Channel& Manager::channel(string name) {
        if ( _channels.find(name) != _channels.end() ) {
          return *(_channels[name]);
        } else {
            throw Exception("Tried to access an unregistered or non-existant channel.");
        }
    }    

    Manager& Manager::watch(string event_name, std::function<void(const Event&)> handler) {
        event_handlers[event_name].push_back(handler);
        return *this;
    }

    Manager& Manager::emit(string event_name, const Event& event) {
        if ( event_handlers.find(event_name) != event_handlers.end() ) {
            for ( auto handler : event_handlers[event_name] ) {
                handler(event);
            }
        }
        return *this;
    }

    //! Apply a function to all processes.
    //! \param f The function to apply. It should take a reference to a process and return void.
    //! \return A reference to the manager, for chaining
    Manager& Manager::all(std::function< void(Process&) > f) {
        for(auto process_ptr : _processes) {
            f(*process_ptr);
        }
        return *this;
    }

    //! Initialize all processes. Usually called before run()
    //! \return A reference to the manager, for chaining
    Manager& Manager::init() {
        return all([](Process& p) { p._init();});
    }

    //! Start all processes. Usually not called directly.
    //! \return A reference to the manager, for chaining
    Manager& Manager::start() {
        return all([this](Process& p) { p._start(_elapsed) ;});
    }    

    //! Stop all processes. Usually not called directly.
    //! \return A reference to the manager, for chaining
    Manager& Manager::stop() {
        return all([](Process& p) { p._stop(); });
    }    

    //! Update all processes if enough time has passed. Usually not called directly.
    //! \return A reference to the manager, for chaining
    Manager& Manager::update() { 
        return all([this](Process& p) {
            if ( _elapsed > p.last_update() + p.period() ) {
                p._update(_elapsed);
            }
        });
    }

    //! Run the manager for the specified amount of time.
    //! \param The desired amount of time to run
    //! \return A reference to the manager, for chaining
    Manager& Manager::run(high_resolution_clock::duration runtime) {

        _start_time = high_resolution_clock::now();
        _elapsed = high_resolution_clock::duration::zero();
        start();        

        while ( _elapsed < runtime ) {
            update();
            _elapsed = high_resolution_clock::now() - _start_time;
        }

        stop();

        return *this;

    }

}