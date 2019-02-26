#include <iostream>
#include <chrono>
#include "elma.h"

//! \file

using namespace std::chrono;
using namespace elma;

class Trigger : public Process {
    public:
    Trigger() : Process("trigger") {}
    void init() {}
    void start() {}
    void update() { 
        std::cout << "switch at " << milli_time() << "\n";
        emit(Event("switch"));
    }
    void stop() {}
};

class Mode : public State {
    public:
    Mode(std::string name) : State(name) {}
    void entry(const Event& e) {
        std::cout << "entering " + name() << "\n";
    }
    void during() {}
    void exit(const Event&) {}
};

int main() {

    Manager m;
    Trigger trigger;
    Mode off("off"), on("on");
    StateMachine fsm("toggle switch");

    fsm.set_initial(off)
       .set_propagate(false)
       .add_transition("switch", off, on)
       .add_transition("switch", on, off);

    m.schedule(trigger, 1_ms)
     .schedule(fsm, 5_ms) // Doesn't matter since mode has empty update()
     .init()
     .run(11_ms);
     
}