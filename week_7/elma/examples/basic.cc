#include <iostream>
#include <chrono>
#include "elma.h"

//! \file

using namespace std::chrono;
using namespace elma;

class MyProcess : public Process {
    public:
    MyProcess(std::string name) : Process(name) {}
    void init() {}
    void start() {}
    void update() { 
        std::cout << name() << ": " 
                << milli_time()
                << "ms\n";
    }
    void stop() {}
};

int main() {
    Manager m;
    MyProcess p("A"), q("B");
    m.schedule(p, 1_ms)
     .schedule(q, 5_ms)
     .init()
     .run(11_ms);
}