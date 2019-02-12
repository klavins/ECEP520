#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <ratio>
#include <thread>

#include "manager.h"
#include "process.h"

using std::string;

namespace {

    class MyProcess : public Process {
      public:
        MyProcess(std::string name) : Process(name) {}
        void init() {
          t = std::chrono::high_resolution_clock::now();
        }
        void start() {}
        void update() { 
          std::chrono::duration<double, std::milli> diff = std::chrono::high_resolution_clock::now() - t;
          std::cout << name() << ": " 
                    << diff.count()
                    << "ms\n";
        }
        void stop() {}
      private:
        time_point<high_resolution_clock> t;
    };

    TEST(Manager,Schedule) {
      Manager m;
      MyProcess p("A"), q("B");
      m.schedule(p, high_resolution_clock::duration(milliseconds(1)))
       .schedule(q, high_resolution_clock::duration(milliseconds(5)))
       .init()
       .run(high_resolution_clock::duration(milliseconds(20)));
    }

}
