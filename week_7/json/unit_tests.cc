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

        double x = (double) j["fib"][0] + (double) j["fib"][1];

        ASSERT_EQ(3.14, j["pi"]);

        auto k = R"(
          {
            "x": [ 1, "x", { "y": 0 } ],
            "y": 3.4
          }
        )"_json;

        ASSERT_EQ(0, k["x"][2]["y"]);
        ASSERT_ANY_THROW(x = k); // Should throw a json type error

        std::cout << "k = " << k << "\n";

    }

}
