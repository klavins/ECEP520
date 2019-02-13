#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <ratio>
#include <thread>

#include "elma.h"

namespace {

    using std::string;
    using namespace std::chrono;

    #define HRC_DUR high_resolution_clock::duration

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

    TEST(Manager,Schedule) {
      elma::Manager m;
      MyProcess p("A"), q("B");
      m.schedule(p, HRC_DUR(milliseconds(1)))
       .schedule(q, HRC_DUR(milliseconds(5)))
       .init()
       .run(HRC_DUR(milliseconds(11)));
       ASSERT_EQ(p.num_updates(), 10);
       ASSERT_EQ(q.num_updates(), 2);
    }

    class OpenLoopCar : public elma::Process {
      public:
        OpenLoopCar(std::string name) : Process(name) {}
        void init() {}
        void start() {
          velocity = 0;
        }
        void update() {
          double u = 1;
          velocity += delta() * ( - k * velocity + u ) / m;
          std::cout << "t: " << milli_time() << " ms\tv: " << velocity << " m/s\n";
        }
        void stop() {}
      private:
        double velocity;
        const double k = 0.02;
        const double m = 100;
    };

    TEST(Car,OpenLoop1) {
      elma::Manager m;
      OpenLoopCar car("Toyota");
      m.schedule(car, HRC_DUR(milliseconds(10)))
       .init()
       .run(HRC_DUR(milliseconds(100)));
    }    

    class ControllableCar : public elma::Process {
      public:
        ControllableCar(std::string name) : Process(name) {}
        void init() {}
        void start() {
          velocity = 0;
        }
        void update() {
          if ( channel("Throttle").nonempty() ) {
            force = channel("Throttle").latest();
          }
          velocity += delta() * ( - k * velocity + force ) / m;
          channel("Velocity").send(velocity);
          std::cout << "t: " << milli_time() << " ms\t" 
                    << " u: " << force << " m^2/s\t"
                    << " v: " << velocity << " m/s\n";
        }
        void stop() {}
      private:
        double velocity;
        double force;
        const double k = 0.02;
        const double m = 100;
    };  

    class CruiseControl : public elma::Process {
      public:
        CruiseControl(std::string name) : Process(name) {}
        void init() {}
        void start() {}
        void update() {
          double v;
          if ( channel("Velocity").nonempty() ) {
            speed = channel("Velocity").latest();
          }
          channel("Throttle").send(-KP*(speed - desired_speed));
        }
        void stop() {}
      private:
        double speed = 0;
        const double desired_speed = 50.0,
                     KP = 0.5;
    };   

    TEST(Car,OpenLoop2) {

      elma::Manager m;

      ControllableCar car("Car");
      CruiseControl cc("Control");
      elma::Channel throttle("Throttle");
      elma::Channel velocity("Velocity");

      m.schedule(car, HRC_DUR(milliseconds(10)))
       .schedule(cc, HRC_DUR(milliseconds(10)))
       .add_channel(throttle)
       .add_channel(velocity)
       .init()
       .run(HRC_DUR(milliseconds(10000)));

    }          

}
