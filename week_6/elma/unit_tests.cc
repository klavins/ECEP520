#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <ratio>
#include <thread>
#include <vector>

#include "elma.h"

namespace {

    using std::string;
    using namespace std::chrono;
    using std::vector;

    #define MS(__ms__) high_resolution_clock::duration(milliseconds(__ms__))

    TEST(RATIO,RATIO) {
      typedef std::ratio<2,3> two_thirds;
      std::cout << two_thirds::num << "/" <<two_thirds::den << "\n";
      typedef std::chrono::duration<int,std::milli> milliseconds_type;

      auto x = milliseconds_type(10);   
      std::cout << "ten ms = " << x.count() << "ms\n";

      typedef std::chrono::duration<double,std::ratio<1,1>> seconds_type;
      auto y = seconds_type(x);
      std::cout << "ten ms = " << y.count() << "s\n";

      auto w = x+y;
      auto z = y+y;
      std::cout << "x+y = " << w.count() << "ms\n";
      std::cout << "y+x = " << z.count() << "s\n";

      auto zero = seconds_type::zero();
      ASSERT_EQ(0, zero.count());

    }

    TEST(CLOCK,CLOCK) {
      high_resolution_clock::time_point t = high_resolution_clock::now();
      std::cout << t.time_since_epoch().count() << " ns since 1970\n";
      typedef std::chrono::duration<double,std::ratio<3600*24*365,1>> years;
      auto y = years(t.time_since_epoch());
      std::cout << y.count() << " hours since 1970\n";
    }

    TEST(CLOCK,DIFF) {
      high_resolution_clock::time_point t1, t2;
      t1 = high_resolution_clock::now();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      t2 = high_resolution_clock::now();
      std::cout << (t2 - t1).count() << " ns\n";      
    }

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
      m.schedule(p, MS(1))
       .schedule(q, MS(5))
       .init()
       .run(MS(11));
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

    TEST(Car,OpenLoop) {
      elma::Manager m;
      OpenLoopCar car("Toyota");
      m.schedule(car, MS(10))
       .init()
       .run(MS(100));
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

    class CruiseControl : public elma::Process {
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

    TEST(Car,ClosedLoop) {

      elma::Manager m;

      ControllableCar car("Car");
      CruiseControl cc("Control");
      elma::Channel throttle("Throttle");
      elma::Channel velocity("Velocity");

      m.schedule(car, MS(10))
       .schedule(cc, MS(10))
       .add_channel(throttle)
       .add_channel(velocity)
       .init()
       .run(MS(10000));

    }

}
