#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <chrono>

#include "process.h"

using std::vector;
using namespace std::chrono;

class Manager {

public:

    Manager();

    Manager& schedule(
        Process& process, 
        high_resolution_clock::duration period);
    
    Manager& drop(Process&);

    Manager& init();
    void update(high_resolution_clock::duration elapsed);
    Manager& run(high_resolution_clock::duration duration);

private:

    vector<Process *> _processes;

};

#endif