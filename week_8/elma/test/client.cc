#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;

    TEST(Client,UrlParts) {
        Client c;
        auto parts = c.url_parts("https://api.github.com/repos/klavins/ecep520");
        ASSERT_EQ("api.github.com", parts.first);
        ASSERT_EQ("/repos/klavins/ecep520", parts.second);
    }

    TEST(Client,Get) {

        Client c;

        for( int i=0; i<3; i++ ) {
          c.get("https://api.github.com/repos/klavins/ecep520", [](json& response) {
              // Note: The body of this function runs in the main thread after
              // c.process_responses is called below.
              ASSERT_EQ("ECEP520", (string) response["name"]);
          });
        }

        // Note: If we don't sleep for a while, then the main thread will complete,
        // killing the children threads started in c.get.
        std::this_thread::sleep_for(std::chrono::seconds(1));

        ASSERT_EQ(3,c.num_responses());
        c.process_responses();

    }

    class GetTester : public Process {
        public:
        GetTester() : Process("Get Tester") {}
        void init() {}
        void start() {
            got_response = false;
            http_get("https://api.github.com/repos/klavins/ecep520", [this](json& response) {
                got_response = true;
            });
        }
        void update() {}
        void stop() {
            ASSERT_EQ(true,got_response);
        }
        bool got_response;
    };

    TEST(Client,ProcessInterface) {
        Manager m;
        GetTester p;
        m.schedule(p,1_ms)
         .init()
         .run(1_s);
    }

}