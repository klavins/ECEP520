Week 8: HTTP in Event Loops
===

Projects
===

Requirements
---

The goal of your project is to build up an example, extension, or other code base drawing from the ideas we have discussed in class and in particular Elma. The overall aspects that *every* project must deliver are:
- Extends or uses Elma
- A new, public Github repo with all the code for your project
- Well documented code including Doxygen generated API descriptions for all classes and methods
- A `README.md` file, to be included on the front page of your github repo with
    - An overview with general usage information 
    - Examples, if applicable
    - Installation instructions
- A Dockerized instance of your code on Dockerhub

Options
---

Below is a list of suggestions for your project. Anything similar to the ones below are fine. 

1. Port Elma to the [Raspberry Pi](https://www.raspberrypi.org/) and build an interesting example using sensors and effectors. 
1. Port Elma to a cell phone and do a similar project.
1. Build a simulated model of a simple robot in [Gazebo](http://gazebosim.org/) and control it using an Elma program to do some simple task, like searching for an object and avoiding objects. Other simulation software that supports C++ is fine too.
1. Port a non-trivial amount of Elma to [Rust](https://www.rust-lang.org/) (e.g. Manager, Process, Event and some examples).
1. Extend the car, cruise control, and driver simulation to use (a) a more realistic simulation of a car's velocity including what gear it is in, wind, and hills; (b) an appropriately tunede PID controller; and (c) a driver that can do interesting things like change gears, speed up, slow down, and stop. Export data taken from simulations and show with plots that the car performs as expected. Consider speeding up the simulation by ignoring real time, making a "simulation mode" version of the `run` method.
1. Extend the robot finite state machine example to include a simulation of the robot in a 2D environment, a simulation of an intruder, a simulation of the recharge station, and a simulation an operator who can send the reset event. Export and visulize data you collect. See previous option for a note on speeding up simulations.
1. Develop a library for checking the *safety* of finite state machines. The library should include a method of the following form:
    ```c++
    check_condition(event_sequence, property)
    ```
    where `event_sequence` is a sequence of events that the machine responds to, and `property` is a function that takes a state machine returns `true` or `false` after each step in along the sequence. Also build a function that produces random sequences of events that you can use in the first argument to do random testing of the property. You need to determine how to represent these ideas as classes and methods. Show your code works with examples similar to the microwave and robot examples.
1. Add bidirectional [UDP](https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html) messages passing from two separate Elma executables that send JSON messages back and forth. UDP messages should show up as events that processes should be able to listen for with handlers. UDP messages should be sent in `update` methods. Build an example or two to show it works.

New Docker Image with HTTP
===

The docker container we have been using has been upgraded to include the httplib library. You can build it from the new Dockerfile is in cppenv-http in this repository with the commands
```bash
cd cppenv-http
docker build -t cppenv-http .
cd ..
docker run -v $PWD:/source -it cppenv-http bash
```
or just use Prof. Klavins' pre-compiled image via Dockerhub with the command
```bash
docker run -v $PWD:/source -it klavins/ecep520:cppenv-http bash
```

Within this container, you can use the JSON library simply by including it.
```c++
#include <httplib/httplib.h>
```

Internet Protocols
===

IP
--

The "Internet Protocols" suite of protocols for communicating among computers on the internet. Raw addresses such as 127.23.4.1 for the basis of how computers find each other via routers and routing tables. IP includes other protocols such as UDP and TCP.

UDP
---

UDP stands for "User Datagram Protocol". It is used for low latency commucation of byte packets and messages between devices on an ethernet and/or the Internet. Messages sent with UDP arrive in order, but are not guaranteed to be delivered. Information such as real time voice and video are common applications. 

TCP
---

TCP stands for "Transmission Control Protocol". It is used for everything else. Messages are broken into packets and sent from one computer to another via routers. Acknowledgments of packet delivery are sent back to the sender. Each packet may take a different route. Failures occur due to over-taxed routers, and result in packets being resent. The rate at which packets are sent is adjusted according to the packet loss rate: every time a packet is lost, the packet send rate is cut in half. Every time a packet is not lost, the rate is incremented by a small amount (up to some limit). 

HTTP
---

HTTP stands for "Hypertext Transfer Protocol". It sits on top of TCP, meaning that it is simply a specification of the format of the messages that are sent back and forth using TCP. It was originally devloped for serving web pages, thus the term hypertext in the name. However, it has developed into a standard for sending data of any kind -- web pages, images, movies, json data, and so on -- in a reliable way. HTTP also specifies how Internet resources are named, using "Uniform Resource Locators" such as http://www.google.com instead of raw IP address.

HTTPS
---

A secure version of HTTP that also uses the "Transport Layer Secutiry" (TLS) or "Secure Socket Layer" (SSL) to communcate. TLS and SSL sit on top of TCP, but encrypt packets before sending them. HTTPS can be either "simple", in which the server publishes a public key, or "mutual" in which case both sides publish public keys. Servers must additionally register with a trusted third party "certificate authority" to prevent spoofing of the service. 

HTTP Details
---

- *Server*: An http server is a program that listens for incoming messages on a "port". Ports are just numbers the operating system uses to direct incomping IP traffic. For example, http usually runs on port 80, https on port 443, telnet on 23, ssh on 22, etc. Servers receive requests for data and "serve" the result. An example of an http server is the Apache Server. But you can make your own http server with a few lines of code.

- *Clients*: An http client is a program that connects to a server (or multiple servers), to request data or to upload data to a server. An example of a client is your web browser. But a robot in a factory or a satillite could also be clients. 

- *Request Format*: Requests are text strings that include
    - A request line, like "GET /klavins/ECEP520 HTTP/1.1" which says to get a particular file or resource from the server.
    - Any number of request header fields (key value pairs). For example, you might add "User-Agent: Elma" so the server knows what kind of client is connecting.
    - An empty line.
    - An optional request body, where you might put data for an upload, etc., encoded as text

- *Response Format*: Responses are similar and include
    - A status, like "HTTP/1.1 200 OK" or "HTTP/1.1 404 Not found"
    - Any numbner of response headers, for example "content-type: json"
    - A blank line
    - A message body encoded as text

- *Get Requests*: A GET request looks like 
    ```
    GET /klavins/ECEP520 HTTP/1.1
    User-Agent: chrome
    ```
    an is used to request a datum with a simple name that can be specified in a path. For example, the one in the previous sentence asks ther server running at github.com to send back the main HTML page for our class' repository.

- *Post Requests*: A POST request might look like like 
    ```
    POST /save HTTP/1.1
    User-Agent: Elma
    content-type: json

    {
        "x": 123.4,
        "y": 234.5,
        "temperature": 34.5
    }
    ```
- *Other Requests*: HTTP also defines PUT, DELETE, and PATCH, which together form the basis of what is called a Restful interface. Rest stands for "Representational State Transfer". For now, GET and POST are sufficient for most purposes, with specific actions like "delete" defined either in the url path or in a JSON message body.

The Curl Client
---

An incredibly useful tool installed on pretty much any unix like environment (including our docker pages) is `curl`. Curl allows you to send get and post requests to servers. For example, to do a simple GET request, do
```bash
curl https://www.google.com
```
will return the httmp for Google's main page.

You can also do a POST request, as in 
```bash
curl -d '{ "x": 123, "y": 234, "temperature": 41.2 }' -X POST localhost:8080/save
```
which we will use in an example below.

More interestingly, you can use Google's data API to get all sorts of interesting data. For example, see [here](https://cloud.google.com/vision/docs/using-curl). Note, charges apply. Other data services at Amazon, NOAA, Github, etc. are similar.

Using HTTP in C++ with [httplib](https://github.com/yhirose/cpp-httplib)
---

C and C++ provide low level access to UDP and TCP with the 'socket' library. An example of low level socket programming can be found [here](https://www.geeksforgeeks.org/socket-programming-cc/). It is a good place to start if you are developing a new protocol on top of TCP or UDP. But for communicating with HTTP, it is too low level, as it does not handle the request and response formats above, and does not know anything about GET and POST.

There are a myriad of libraries for dealing with HTTP in C++. A nice one that has a clean interface is called `cpp-httplib`. I have made a branch of this library and started to hack it a bit for use with Elma [here](https://github.com/klavins/cpp-httplib). In particular, I have added an exception class and started to add exception throws to the library, which uses return values to denote errors instead of exceptions (a very old C idea).

Examples of how to use cpp-httplib to make a simple server and client are [here](https://github.com/klavins/cpp-httplib).

A Simple Database Server
---

As an example of what a server might do in an embedded systems or robotics setting, consider a scenario where a number of robots are exploring or surveying an area. Say they each have temperature sensors. To get a global picture of how the temperature is changing over space and time, the robots could upload their coordinates and the temperature to a server. To provide this service this, we will make a server that responds to two types of request. 

- A POST request of the form `{ "x": 123, "y": 234, "temperature": 41.2 }` sent to the route `/save`
- A GET request to routes of the form `/find/42` that, in this case, returns the 42nd temperature reading

A simple database like datastructue is defined as follows:
```c++
std::map<int, // id
    std::tuple<
       int,      // timestamp
       double,   // x position
       double,   // y position
       double    // temperature
    >
> database;
```
To respond to the POST requests, we 
- Parse the JSON request (returning an error if it is malformed)
- Update the database
- Respond to the client with information about what was done
In code, this looks like
```c++
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
```
To respond to GET requests, we look up the id in the database, and return the resulting tuple in json.
```c++
svr.Get(R"(/find/(\d+))", [&](const Request& req, Response& res) {
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
    }
    res.set_content(result.dump(), "json");
});
```

 

To test the server, we'll map the port 8080 in the docker container to the port 80 on the host, by restarting the docker container with
```bash
docker run -p 8080:8080/tcp -v $PWD:/source -it klavins/ecep520:cppenv-http bash
```
Note that the new Dockerfile also includes a directive to expose 8080 to the host with `EXPOSE 8080`. 

Next, do `make bin/temperature_server` in the examples directory and then run `bin/temperature_server`. This will tie up the terminal, so you'll need another terminal to test it. If you have a Mac, you can just open a terminal and run
```bash
curl -d '{ "x": 123, "y": 234, "temperature": 41.2 }' -X POST localhost:8080/save
```
or
```c++
curl localhost:8080/find/1
```

Adding HTTP to Elma
===

It is tempting to add cpp-httplib directly to Elma and allow processes to use the `Get` method it provides for Clients directory. However, doing so raises two issues:
- The `Get` method of cpp-httplib blocks. That is, until the data is sent from the client to the server and back, nothing happens in the client program. Depending on how long this takes, it will block all other processes' `update` methods.
- We don't want the callback function for `Get` to run at arbitrary times (like in the middle of an update), but rather at a specified time in the main loop, like just before all processes are updated.
- We might decide to swap out cpp-httplib later, so we should define our own interface to http for Elma so that the backend can be easily changed later.

The third issue is easily solved by creating a new class. But the first two issues requires some new concepts.

Threads
---

When you want multiple things to happen at once in C or C++, such as updating processes and waiting for a TCP response, you use [std::thread](http://www.cplusplus.com/reference/thread/thread/). Use `#include <thread>`. Here is an example:
```c++
void do_something(int x, int y) {
    // suppose this takes a while
    // and we don't need the result
}

int main() {

  std::thread t1(do_something, 1, 2); 
  std::thread t2(do_something, 1, 2); 

  t1.detach();
  t2.detach();

  // continue on doing something else
  // note: if the main function ends, it will
  // stop the threads, even if they haven't
  // completed.

  return 0;

}
```
You can also wait for the threads to complete, as with
```c++
int main() {

  std::thread t1(do_something, 1, 2); 
  std::thread t2(do_something, 1, 2); 

  // continue on doing stuff

  t1.join();
  t2.join();

  return 0;

}
```
In general, use `detach` if you don't intend to `join` them later. 

Mutex
---
One issue with threaded code is the possibility of *race conditions*, in which two or more processes access the same data structures in different orders depending on timing. Here is an example:
```c++
char buffer[301];
int index = 0;

void print_block (int n, char c) {
    for (int i=0; i<n; ++i) { 
    buffer[index] = c;
    index++;
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%5));
    }
}

TEST(MUTEX,MUTEX) {

    buffer[300] = 0;
    std::thread t1 (print_block,150,'*');
    std::thread t2 (print_block,150,'$');
    t1.join();
    t2.join();
    auto desired_result = std::string(150,'*') + std::string(150,'$');
    ASSERT_STREQ(desired_result.c_str(), buffer);

}
```
Running this test may sometimes work, but more often than not you will get an failed test report:
```
Expected equality of these values:
  desired_result.c_str()
    Which is: "******************************************************************************************************************************************************$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
  buffer
    Which is: "*$$**$$$*$$$**$*$$*$$*$*$$$*$$$**$*$*$$*$*$$**$*$$$$**$*$$*$$$****$*$*$$**$$*$***$$*$*$*$*$$$*$**$*$$$**"
```

To fix this, we use a `mutex` (use `#include <mutex>`), which is a mutual exclusion lock. When a thread locks a mutex, it will be allowed to contiune with what it is doing without being interrupted until it unlocks the mutex. The fix for the above code is
```c++
std::mutex mtx;
char buffer[301];
int index = 0;

void print_block (int n, char c) {
    mtx.lock();
    for (int i=0; i<n; ++i) { 
    buffer[index] = c;
    index++;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    mtx.unlock();
}
```
which enables the test to pass.

A Client Class for Elma
---
For our Client class, we will provde a similar interface to cpp-httplib, but with different behavior:
- Get requests will look like
    ```c++
    Client& get(std::string url, std::function<void(json&)> handler);
    ```
    We will pass the entire URL and not specifiy the host and path separately.
    We will expect a JSON result from the server, which our implementation will parse and supply to the handler.
- Our client's `get` method will be **non-blocking** so that it doesn't block the `Manager` main loop.
- The `get` method will be run in a thread that does the following:
    - Parse the result. 
    - Put it and a reference to the handler into a list of responses
- The `Manager` will then dispatch all new responses to their handlers each time through the main loop.

Here are some implemention highlights.

The `Client` class has a private variable to store responses:
```c++
// Client.h
private:
std::vector<std::tuple<json, std::function<void(json&)>>> _responses;
```

The `get` method is
```c++
Client& Client::get(std::string url, std::function<void(json&)> handler) {
    std::thread t(&Client::_get_thread,this,url,handler);
    t.detach(); // detaching means we don't have to join later
}
```
This launches a thread with the private method `_get_thread` running on it. This private method does all the work. 
```c++
    void Client::_get_thread(std::string url, std::function<void(json&)> handler) {

        json json_response;

        try {

            auto response = _get_aux(url);

            if (response && response->status == 200) {
                json_response = json::parse(response->body);
             } else if ( response ) {
                std::cout << "Warning:: Elma client connected to a server that returned Error: " 
                          << response->status 
                          << std::endl;
            } else {
                std::cout << "Warning:: Elma client returned no result\n" ;
            }

        } catch (const httplib::Exception& e) {
            std::cout << "Warning: Elma client failed: " << e.what() << "\n";
        } catch(const json::exception& e ) {
            std::cout << "Warning: Elma client could not parse response: " << e.what() << "\n";
        } catch (...) {
            std::cout << "Warning: Elma client failed with no message\n";
        }

        _mtx.lock();
        _responses.push_back(std::make_tuple(json_response, handler));
        _mtx.unlock();

    }  
```
Note that we do not throw errors in the thread, because the calling process would not be able to catch them. Instead, we print issues out to the terminal in hopes that our user will notice them.

Our `Client` class is smart about how whether to use HTTP or HTTPS by parsing the url sent to it. The parse function has in it code that reads:
```c++
if ( protocol == "http" ) {
    _use_ssl = false;
} else if ( protocol == "https" ) {
    _use_ssl = true;
} else {
    throw Exception("Protocol " + protocol + " not implemented in Client.");
} 
```
We then supply a method that returns the desired client type with:
```c++
const std::shared_ptr<httplib::Response> Client::_get_aux(std::string url) {
    auto parts = url_parts(url); // url parser that also sets _use_ssl
    if ( _use_ssl ) {
        httplib::SSLClient cli(parts.first.c_str(), 443);
        return cli.Get(parts.second.c_str());              
    } else {
        httplib::Client cli(parts.first.c_str(), 80);
        return cli.Get(parts.second.c_str());              
    }        
}
```

To process all requests, the `Client` class provides the method:
```c++
Client& Client::process_responses() {

    _mtx.lock();
    for(auto response : _responses ) {
        std::get<1>(response)(std::get<0>(response));
    }
    _responses.clear();
    _mtx.unlock();
    return *this;

}
```

The `Manager` now includes a `Client` instance variable and accessor:
```c++
// Manager.h

public:
Client& client() { return _client; }

private:
Client _client;
```
And in its `update` method it now does
```c++
_client.process_responses();
```
Finally, so that a `Process` can access the `get` method, we add a method
```c++
void Process::http_get(std::string url, std::function<void(json&)> handler) {
    _manager_ptr->client().get(url,handler);
}
```
As an example, see the file `test/client.cc`. 

Exercises
===
1. Add a GET response `/above/(\d+)` the temperature_server example that returns all database entries whose temperature exceeds a given integer valued threshold. It should a JSON array of all the tuples it finds, also in JSON format (similar to `/find`) that meet the criterion.
1. Add a `post` request method to the Elma `Client` class. It should have the form
    ```c++
    Client& post(std::string url, json data, std::function<void(json&)> handler);
    ```
    The method `process_responses()` should dispatch the resulting responses from the server. And `num_responses()` should return the total GET and POST responses. Test your method by making an example that works with the temperature server.
1. Decide on a project topic and do the following:
    - Create a new public github repo for your project. Send the name of the repo to the TAs, whoe will keep a list of all repos.
    - Create a README.md file in your repo that describes your project goals in detail. What will you do? What will it mean for you to succeed? What resources will you use (e.g. Elma, other C++ libraries, robots, etc.)
    - Create a set of at least five milestones that you will achieve, including completing the final project, and state when you will achieve them. A milestone could be as simple as getting particular test or code library to work, or completing the API documentation for your project.
