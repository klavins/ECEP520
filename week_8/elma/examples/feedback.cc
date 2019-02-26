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
        std::cout << "t: "  << milli_time() << " ms\t" 
                << " u: " << force        << " N\t"
                << " v: " << velocity     << " m/s\n";
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
    void init() {}
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
    const double desired_speed = 50.0,
                    KP = 314.15;
                    vector<double> _v;
};   

int main() {

    Manager m;

    Car car("Car");
    CruiseControl cc("Control");
    Channel throttle("Throttle");
    Channel velocity("Velocity");

    m.schedule(car, 10_ms)
    .schedule(cc, 10_ms)
    .add_channel(throttle)
    .add_channel(velocity)
    .init()
    .run(1000_ms);

}