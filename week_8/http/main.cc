#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>

#include <gtest/gtest.h>
#include <json/json.h>
#include <iostream>
#include <string>
#include <vector>

GTEST_API_ int main(int argc, char **argv) {

    using nlohmann::json; 
    json result;

    httplib::SSLClient cli("api.github.com", 443);
    auto res = cli.Get("/users/klavinslab/repos");

    if (res && res->status == 200) {
        std::cout << res->body << std::endl;
        result = json::parse(res->body);
    } else if ( res ) {
        std::cout << "server responded with an error: " + res->status << std::endl;
    } else {
        std::cout << "no result" << std::endl;
    }

    for ( auto &repo : result ) {
      std::string name = repo["name"];
      std::cout << repo["id"] << ": " << name << "\n";
    }

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();    

}
