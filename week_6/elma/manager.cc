#include <stdexcept>
#include <iostream>
#include "elma.h"

namespace elma {

    Manager& Manager::schedule(
        Process& process, 
        high_resolution_clock::duration period) {

        process._period = period;
        _processes.push_back(&process); 
        process._manager_ptr = this;            

        return *this;

    }

    Manager& Manager::drop(Process&) {

    }

    Manager& Manager::add_channel(Channel& channel) {
        _channels[channel.name()] = &channel;
        return *this;
    }

    Channel& Manager::channel(string name) {
        if ( _channels.find(name) != _channels.end() ) {
          return *(_channels[name]);
        } else {
            throw std::domain_error("Tried to access an unregistered or non-existant channel.");
        }
    }

    Manager& Manager::all(std::function< void(Process&) > f) {
        for(auto process_ptr : _processes) {
            f(*process_ptr);
        }
        return *this;
    }

    Manager& Manager::init() {
        return all([](Process& p) { p._init();});
    }

    Manager& Manager::start() {
        return all([this](Process& p) { p._start(_elapsed) ;});
    }    

    Manager& Manager::stop() {
        return all([](Process& p) { p._stop(); });
    }    

    Manager& Manager::update() { 
        return all([this](Process& p) {
            if ( _elapsed > p.last_update() + p.period() ) {
                p._update(_elapsed);
            }
        });
    }

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