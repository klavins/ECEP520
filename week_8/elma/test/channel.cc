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
        TestProcess(string name) : Process(name) {}
        void init() {}
        void start() {}
        void update() {

            auto ch = channel("test channel");

            ASSERT_EQ("test channel", ch.name());
            ASSERT_EQ(0, ch.size());

            ch.send("hello");
            ASSERT_EQ("hello", ch.latest());
            ASSERT_EQ(false, ch.empty());
            ASSERT_EQ(true, ch.nonempty());     

            ch.send(3.14159);
            ASSERT_EQ(3.14159, ch.latest());                   

            ch.send(R"({ "num": 1.23, "name": "elma", "vals": [1,2,3] })"_json);
            auto j = ch.latest();

            ASSERT_EQ(1.23, j["num"]);
            ASSERT_EQ("elma", j["name"]);
            ASSERT_EQ(vector<int>({1,2,3}), j["vals"]);

            ASSERT_EQ("hello", ch.earliest());   
            ch.flush();
            ASSERT_EQ(true, ch.empty());

        }
        void stop() {}
    };    

    TEST(Channel,SendRecv) {
        TestProcess tp("test process");
        Channel c("test channel");
        Manager m;
        m.add_channel(c)
         .schedule(tp, 10_ms);
        tp.update();
    }

    TEST(Channel,Capacity) {
        Channel c("test channel",50);
        Manager m;
        m.add_channel(c);
        for ( int i=0; i<100; i++ ) {
            m.channel("test channel").send(i);
        }
        ASSERT_EQ(50, m.channel("test channel").size());
    }


    class BadProcess : public Process {
        public:
        BadProcess(string name) : Process(name) {
            // You can't access channels from the constructor since
            // neither the process or the channel have been added to the
            // manager yet.
            double x = channel("test").size();
        }
        void init() {}
        void start() {}
        void update() {}
        void stop() {}
    };  

    TEST(Channel,Errors) {
        Channel c("test");
        ASSERT_THROW(BadProcess p("bad"),Exception);
    }

}