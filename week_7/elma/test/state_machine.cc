#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    class Mode : public State {
        public:
        Mode(std::string name) : State(name) {}
        void entry(const Event& e) {
            std::cout << "entering " + name() << "\n";
        }
        void during() {}
        void exit(const Event&) {}
    };

    TEST(StateMachine,NoInitial) {
        Manager m;
        StateMachine fsm;
        Mode a("a"), b("b");
        fsm.add_transition("event", a, b);
        m.schedule(fsm,10_ms)
         .init();

        ASSERT_THROW(m.run(100_ms),Exception);
    }

}