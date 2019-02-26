#include <iostream>
#include <chrono>
#include "elma.h"

//! \file

using namespace std::chrono;
using std::vector;
using namespace elma;

class Car : public Process {
    public:
    Car(std::string name) : Process(name) {}
    void init() {}
    void start() {
        velocity = 0;
    }
    void update() {
        if ( channel("Throttle").nonempty() ) {
            force = channel("Throttle").latest();
        }
        velocity += ( delta() / 1000 ) * ( - k * velocity + force ) / m;
        channel("Velocity").send(velocity);
        std::cout << milli_time() << ","
                  << velocity << " \n";
    }
    void stop() {}
    private:
    double velocity;
    double force;
    const double k = 0.02;
    const double m = 1000;
};  

class CruiseControl : public Process {
    public:
    CruiseControl(std::string name) : Process(name) {}
    void init() {
        watch("desired speed", [this](Event& e) {
            desired_speed = e.value();
        });
    }
    void start() {}
    void update() {
        if ( channel("Velocity").nonempty() ) {
            speed = channel("Velocity").latest();
        }
        channel("Throttle").send(-KP*(speed - desired_speed));
    }
    void stop() {}
    private:
    double speed = 0;
    double desired_speed = 0.0;
    const double KP = 314.15;
                 vector<double> _v;
};

class Driver : public Process {

    public: 
    Driver(std::string name) : Process(name) {}
    void init() {
        desired_speed = 50;
    }
    void start() {}
    void update() {
        if ( desired_speed == 50 ) {
            desired_speed = 60;
        } else {
            desired_speed = 50;
        }
        emit(Event("desired speed", desired_speed));
    }
    void stop() {}
    private:
    double desired_speed;

};

int main() {

    Manager m;

    Car car("Car");
    CruiseControl cc("Control");
    Driver driver("Steve");
    Channel throttle("Throttle");
    Channel velocity("Velocity");

    m.schedule(car, 100_ms)
    .schedule(cc, 100_ms)
    .schedule(driver, 5_s)
    .add_channel(throttle)
    .add_channel(velocity)
    .init()
    .run(40_s);

}