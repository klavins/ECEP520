#include <iostream>
#include "manager.h"

Manager::Manager() {

}

Manager& Manager::schedule(
    Process& process, 
    high_resolution_clock::duration period) {

        process._period = period;
        _processes.push_back(&process); // note: without pointers, calling push_back would
                                        // try to copy process, which was declared as a Process
                                        // which is an abstract base class, and can't be constructed

    return *this;

}

Manager& Manager::drop(Process&) {

}

Manager& Manager::init() {

    for(auto process_ptr : _processes) {
        process_ptr->_last_update = high_resolution_clock::duration::zero();
        process_ptr->_status = Process::INITIALIZED;
        process_ptr->init();
    }

    return *this;    

}

void Manager::update(high_resolution_clock::duration elapsed) { 

    for(auto process_ptr : _processes) {

        if ( elapsed > process_ptr->last_update() + process_ptr->period() ) {
            process_ptr->update();
            process_ptr->_last_update = elapsed;
        }
    }

    return;

}

Manager& Manager::run(high_resolution_clock::duration duration) {

    auto t0 = high_resolution_clock::now();
    auto elapsed = high_resolution_clock::duration::zero();

    // std::cout << elapsed.count() << "\n";
    while ( elapsed < duration ) {

        update(elapsed);
        elapsed = high_resolution_clock::now() - t0;
        // std::cout << elapsed.count() << "\n";

    }

    return *this;

}