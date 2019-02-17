#include "gtest/gtest.h"
#include "elma.h"
#include <iostream>

#define MS(__ms__) high_resolution_clock::duration(milliseconds(__ms__))

namespace {

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

    TEST(Basic,Basic) {
        elma::Manager m;
        MyProcess p("P");
        m.schedule(p,MS(10));

    }

}
