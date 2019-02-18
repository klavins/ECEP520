#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    class TestProcess : public Process {
        public:
        TestProcess(std::string name) : Process(name) {}
        void init() {
            watch("hello", [this](const Event& e) { 
                str = e.value();
            });
            watch("pi", [this](const Event& e) { 
                x = e.value();
            });            
        }
        void start() {}
        void update() { 
            emit("hello", Event("world"));
            ASSERT_EQ("world", str);
            emit("pi", Event(3.14));
            ASSERT_EQ(3.14, x);
        }
        void stop() {}
        private:
        string str;
        double x;
    };

    TEST(Basic,Basic) {
        TestProcess p("P");        
        Manager m;
        m.schedule(p,MS(10))
          .init();
        p.update();
    }

}