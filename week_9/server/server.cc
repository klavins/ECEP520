#include "httplib/httplib.h"
#include "json/json.h"
#include <iostream>
#include <ctime>
#include <map>
#include <string>

long int unix_timestamp() {
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}

int main(void)
{
    using namespace httplib;
    using nlohmann::json; 

    std::cout << "Starting server\n";

    Server svr;

    // Example databased for saving temperatures recorded at specific x, y locations
    // This should be at least replaced with a Database class
    // Or better yet, use an actual database like sqlite, mysql, postgress, etc.
    //  id              timestamp  x       y       temperature
    std::map<int, std::tuple<int,       double, double, double>> database;
    int next_id = 0;

    svr.Post("/save", [&](const Request& req, Response& res) { 

        json request, result;

        try {
            request = json::parse(req.body);
        } catch(json::exception e) {
            result["result"] = "error";
            result["message"] = e.what();
            res.set_content(result.dump(), "json");
            return;
        }

        std::cout << "Got new save request " << request.dump() << std::endl;  

        database[next_id] = std::make_tuple(
          unix_timestamp(),
          request["x"].is_number() ? (double) request["x"] : 0,
          request["y"].is_number() ? (double) request["y"] : 0,          
          request["temperature"].is_number() ? (double) request["temperature"] : 0
        );

        result["result"] = "ok";
        result["id"] = next_id++;
        res.set_content(result.dump(), "json");

    });

    svr.Get(R"(/find/(\d+))", [&](const Request& req, Response& res) {
        std::cout << "Got find request for id = " << req.matches[1] << "\n";
        auto id = std::stoi(req.matches[1].str());
        json result;
        if ( database.find(id) != database.end() ) {
            result = { 
                { "result", "ok" },
                { "id", id },
                { "timestamp", std::get<0>(database[id]) },
                { "x", std::get<1>(database[id]) },
                { "y", std::get<2>(database[id]) },
                { "temperature", std::get<3>(database[id]) }                                      
            };
        } else {
            result["result"] = "error";
            result["message"] = "not found";
            res.status = 404;
        }
        res.set_content(result.dump(), "json");
    });

    svr.listen("0.0.0.0", 80); // Note, only this port is exposed to 
                                 // host machine

    std::cout << "You can't see me, because svr.listen never returns\n";

}

