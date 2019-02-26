#include <iostream>
#include <chrono>
#include "gtest/gtest.h"
#include "elma.h"

//! \file

using namespace std::chrono;
using namespace elma;

class Power : public Process {

    public:
    Power() : Process("power") {}
    void init() {
        watch("on", [this](Event& e) {
            running = true;
        });
        watch("off", [this](Event &e) {
            running = false;
        });
        watch("set power", [this](Event& e) {
            power_level = e.value();
        });
    }
    void start() {
        running = false;
        power_level = 0;
    }
    void update() {}
    void stop() {}

    bool running;
    double power_level;

};

class DoorClosedOff : public State {
    public:
    DoorClosedOff() : State("Door closed off") {}
    void entry(const Event& e) {
        if ( e.name() == "power level set" ) {
            emit(Event("set power", e.value()));
        }
    }
    void during() {} 
    void exit(const Event& e) {
        if ( e.name() == "on button pressed" ) {
            emit(Event("on"));
        }        
    }
};

class DoorClosedOn : public State {
    public:
    DoorClosedOn() : State("Door closed on") {}
    void entry(const Event& e) {}
    void during() {} 
    void exit(const Event& e) {
        emit(Event("off"));
    }
};

class DoorOpen : public State {
    public:
    DoorOpen() : State("Door open") {}
    void entry(const Event& e) {
        if ( e.name() == "power level set" ) {
            emit(Event("set power", e.value()));
        }           
    }
    void during() {} 
    void exit(const Event& e) {}
};

TEST(Microwave, Safety) {

    Manager m;

    Power power;

    DoorClosedOff closed_off;
    DoorClosedOn closed_on;
    DoorOpen open;        

    StateMachine microwave;

    microwave
      .set_initial(closed_off)
      .add_transition("on button pressed", closed_off, closed_on)
      .add_transition("door opened", closed_off, open)
      .add_transition("power level set", closed_off, closed_off)

      .add_transition("off button pressed", closed_on, closed_off)
      .add_transition("door opened", closed_on, open)

      .add_transition("door closed", open, closed_off)
      .add_transition("power level set", open, open);

    m.schedule(power, 10_ms)
      .schedule(microwave, 10_ms)
      .init();

    m.start();
    m.emit(Event("on button pressed"));
    ASSERT_EQ(true, power.running);

    m.start();
    m.emit(Event("door opened"));    
    m.emit(Event("on button pressed"));
    ASSERT_EQ(false, power.running); 

    // INSERT GRADING TESTS HERE 

}

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}