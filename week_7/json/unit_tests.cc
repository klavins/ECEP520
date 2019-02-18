#include <gtest/gtest.h>
#include <json/json.h>
#include <iostream>

namespace {

    using nlohmann::json; 

    TEST(Json,Construct) {

        json j;
        j["pi"] = 3.14;
        j["fib"] = { 1, 1, 2, 3, 5, 8, 13 };
        j["me"] = { { "first", "eric" }, { "last", "klavins"} };
        std::cout << j.dump() << "\n";

        ASSERT_EQ(3.14, j["pi"]);

        auto k = R"(
          {
            "x": [ 1, "x", { "y": 0 } ],
            "y": 3.4
          }
        )"_json;

        std::cout << k["x"][2]["y"].dump() << "\n";

        ASSERT_EQ(0, k["x"][2]["y"]);        

    }

}