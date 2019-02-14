Week 6: Event Loop Management
===

Many embedded systems run *event loops*, which are essentially endless while loops of the following form:

    while alive
        read sensors
        compute control values
        send values
        check for and respond to evens
    end

In fact, I have seen event loops in which thousands of lines of sequential code was put inside `main()` in a while loop. While such an arrangment may be acceptable for small projects, big projects with many contributors need more structure.

Starting this week, then, we will be building an Event Loop MAnager called *elma*, which includes (or will include), support for:
- Defining modular processes
- Scheduling a process to run at a desired frequency
- Communicating among processes with *channels*
- Communicating among processes with *events* (next week).

Note that *elma* is a completely new project, being developed for this course. Thus, it will have bugs, missing features, poor documentation, and low test coverage. However, we as a class will be building *elma* week by week.

Overview
===

Before we delve into the details of how *elma* works, here is an example of how to use *elma* to define a simple cruse control class. First, we define a new process.
```c++
  class CruiseControl : public elma::Process {
    public:
      CruiseControl(std::string name) : Process(name) {}
      void init() {}
      void start() {}
      void update() {
        if ( channel("Velocity").nonempty() ) {
          speed = channel("Velocity").latest();
        }
        channel("Throttle").send(-KP*(speed - desired_speed));
      }
      void stop() {}
    private:
      double speed = 0;
      const double desired_speed = 50.0,
                   KP = 0.5;
  }; 
```
This process *inherits* code from a base `Process` class, which will describe later. It defines several required methods: `init`, `start`, `update`, and `stop`. The only method that does anything is the `update` method, which reads a value from a `Velocity` channel (presumbaly connected to the car's sensor), computes a control law, and sends the result via a `Throttle` channel to the car. 

To define an event loop using this process, you would do something like:
```c++
// Declare a new process manager
elma::Manager m;

// Declare processes
ControllableCar car("Car"); // class definition not shown here
CruiseControl cc("Control");
elma::Channel throttle("Throttle");
elma::Channel velocity("Velocity");

m.schedule(car, HRC_DUR(milliseconds(10))) // car updates every 10ms
  .schedule(cc, HRC_DUR(milliseconds(10))) // controller updates every 10ms
  .add_channel(throttle)                   // register the throttle channel
  .add_channel(velocity)                   // register the velocity channel
  .init()                                  // initialize all processes
  .run(HRC_DUR(milliseconds(10000)));      // run for 10s
```

The design of *elma* requires a few concepts we have not yet discussed:

- Real time management
- Inheritance

We will cover these first, and then show how we use these concepts.

Ratios and Durations
===

One of the big problems in code that uses times is that it is hard to remember (and for others to figure out) what units of time you are using at any given point in the code. This is because if you represent a time point or a duration as an integer, there is no associated information about what the units are. The `chrono` library solves this with a new templated class called `duration`, which in turn uses `ratio`. 

A `std::ratio` allows you to represent contant ratios as types. For example, to represention the ratio two thirds, you would write
```c++
#include <ratio>
typedef std::ratio<2,3> two_thirds;
std::cout << two_thirds::num << "/" <<two_thirds::den << "\n"; 
```
Built into the library are a number of predefined ratios, such as `std::milli` or `std::kilo`, which represent the ratios 1:1000 and 1000:1 respectively.

A `std::duration` is a time span template that takes a `count` type and a `period` ratio (in seconds). For example, a millisecond duration type is defined by
```c++
typedef std::chrono::duration<int,std::milli> milliseconds_type;
```
which specifies that a value of type `milliseconds_type` will give you a duration in milliseconds accurate to 1/1000 of a second as an integer. You can make a duration of 10ms as follows:
```c++
auto x = milliseconds_type(10);
```
You can covert to a different representation by sending a duration to the constructor of another duration. For example, to express 10ms in seconds (as 0.01s), you can do
```c++
typedef std::chrono::duration<double,std::ratio<1,1>> seconds_type;
auto y = seconds_type(x);
std::cout << "ten ms = " << x.count() << "ms\n"; // prints 10 ms
std::cout << "ten ms = " << y.count() << "s\n";  // prints 0.01s
```
Note that we can't print a `duration` directly, as it is a complex type. Instead, we call the `count()` method, which returns the number of periods in the duration. The number of periods is, of course, dependent on the size of the period.

Durations have standard arithmetic defined on them. The result of adding to durations is to get, not surprisingly, the sum of the durations. For example, the expression
```c++
auto z = x+y; // 20 ms
```
adds the two durations together to get a new duration. Note that in this case `x` and `y` have different period sizes and counts. According to the specification, "when two duration objects of different types are involved, the one with the longest period (as determined by common_type) is converted before the operation." Thus, y is converted to `milliseconds_type` and then added to `x` to get `z`. 

There is a special duration, zero, which can be used as follows:
```c++
auto z = seconds_type::zero();
```

Clocks and Timepoints
===

The goal is for *elma* to manage when processes execute at approximately millisecond resolution, and allow the use to specify when things should happen. Thus, we will need to
- Know what time it is
- Add an subtract times
- Convert between different units of time

So that we know what timeit is, the `chrono` library provides several clocks. We will use the `high_resolution_clock`, which one most systems will have a resolution of one nanosecond. To get the current time with the clock, you write
```c++
high_resolution_clock::time_point t = high_resolution_clock::now();
```
In this case `t` represents a the current time, but it isn't much use unless you use it relative to some other time. For example, you can ask for the amount of time since 1970 (known as the beginning of time for computers) via:
```c++
std::cout << t.time_since_epoch().count() << " ns since 1970\n";
typedef std::chrono::duration<double,std::ratio<3600*24*365,1>> years;
auto y = years(t.time_since_epoch());
std::cout << y.count() << " years since 1970\n";
```
which prints something like
```bash
1550117813447212300 ns since 1970
49.1539 hours since 1970
```
This may not be of much use. More likely, you would use timepoints to mark different times in the execution of a program. For example,
```c++
high_resolution_clock::time_point t1, t2;
t1 = high_resolution_clock::now();
f(); // a function that might take a while
t2 = high_resolution_clock::now();
std::cout << "f took " << (t2 - t1).count() << " ns\n";
```
Note that the `-` operator on `time_points` is defined to return a `duration`.

Processes
===

Process Management
===

Channels
===

Example: Cruise Control
===

Exercises
===

1. Develop a StopWatch class that can be used to time functions. The class should have the following methods:
    ```c++
    void start();              // starts the timer
    void stop();               // stops the timer
    void reset();              // sets stopwatch to zero
    double get_minutes();      // number of minutes counted
    double get_seconds();      // number of seconds counted
    double get_milliseconds(); // number of milliseconds counted
    ```
    Here is an example usage:
    ```c++
    #define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(1500))

    Stopwatch w; // should set the stopwatch to 0 seconds
    w.start(); 
    SLEEP;
    w.stop();    
    std::cout << w.get_seconds() << "\n"; // about 1.5
    SLEEP;
    std::cout << w.get_seconds() << "\n"; // still about 1.0 since w was stopped
    w.start(); 
    SLEEP;
    w.stop();    
    std::cout << w.get_seconds() << "\n"; // about 3.0
    w.reset();
    std::cout << w.get_seconds() << "\n"; // 0.0
    ```
    For testing your method, we will use assertions that test that `get_seconds` (for example) is approximately equal to the number of seconds that the stopwatch should have counted.

