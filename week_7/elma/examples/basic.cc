#include <iostream>
#include <chrono>
#include "elma.h"

using namespace std::chrono;

class MyProcess : public elma::Process {
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
    elma::Manager m;
    MyProcess p("A"), q("B");
    m.schedule(p, MS(1))
     .schedule(q, MS(5))
     .init()
     .run(MS(11));
}