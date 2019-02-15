Week 6: Event Loop Management
===

Many embedded systems run *event loops*, which are essentially endless while loops of the following form:

    while alive
        read sensors
        compute control values
        send values
        check for and respond to events
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
This process *inherits* code from a base `Process` class, which wewill describe later. It defines several required methods: `init`, `start`, `update`, and `stop`. The only method that does anything is the `update` method, which reads a value from a `Velocity` channel (presumbaly connected to the car's sensor), computes a control law, and sends the result via a `Throttle` channel to the car. 

To define an event loop using this process, you would do something like:
```c++
// Declare a new process manager
elma::Manager m;

// Declare processes
Car car("Car"); // class definition not shown here
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

- Time
- Inheritance
- Process management
- Interprocess communication

We will cover these first, and then show how we use these concepts.

Ratios and Durations
===

One of the big problems in code that uses time is that it is hard to remember (and for others to figure out) what units of time you are using at any given point in the code. This is because if you represent a time point or a duration as an integer, there is no associated information about what the units are. The [chrono](http://www.cplusplus.com/reference/chrono/) library solves this with a new templated class called `duration`, which in turn uses `ratio`. 

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
You can convert to a different representation by sending a duration to the constructor of another duration. For example, to express 10ms in seconds (as 0.01s), you can do
```c++
typedef std::chrono::duration<double,std::ratio<1,1>> seconds_type;
auto y = seconds_type(x);
std::cout << "ten ms = " << x.count() << "ms\n"; // prints 10 ms
std::cout << "ten ms = " << y.count() << "s\n";  // prints 0.01s
```
Note that we can't print a `duration` directly, as it is a complex type. Instead, we call the `count()` method, which returns the number of periods in the duration. The number of periods is, of course, dependent on the size of the period.

Durations have standard arithmetic defined on them. The result of adding two durations is to get, not surprisingly, the sum of the durations. For example, the expression
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

The goal is for *elma* to manage when processes execute at approximately millisecond resolution, and allow them to specify when things should happen. Thus, we will need to
- Know what time it is
- Add and subtract times
- Convert between different units of time

So that we know what time it is, the `chrono` library provides several clocks. We will use the `high_resolution_clock`, which on most systems will have a resolution of one nanosecond. To get the current time with the clock, you write
```c++
high_resolution_clock::time_point t = high_resolution_clock::now();
```
In this case `t` represents the current time, but it isn't much use unless you use it relative to some other time. For example, you can ask for the amount of time since 1970 (known as the beginning of time for computers) via:
```c++
std::cout << t.time_since_epoch().count() << " ns since 1970\n";
typedef std::chrono::duration<double,std::ratio<3600*24*365,1>> years;
auto y = years(t.time_since_epoch());
std::cout << y.count() << " years since 1970\n";
```
which prints something like
```bash
1550117813447212300 ns since 1970
49.1539 years since 1970
```
This may not be of much use. More likely, you would use timepoints to mark different times in the execution of a program. For example,
```c++
high_resolution_clock::time_point t1, t2;
t1 = high_resolution_clock::now();
f(); // a function that might take a while
t2 = high_resolution_clock::now();
std::cout << "f took " << (t2 - t1).count() << " ns\n";
```
Note that the "`-`" operator on `time_points` is defined to return a `duration`.

Inheritance
===

In *elma*, our users will write their own process classes that the process manager will manage. In particular, the manager will keep a list of processes and apply the same methods to all of them. There are two problems to solve with this arrangment:
- Given that lists in C++ have to be all of the same type, how can the manager keep a list of different process types?
- How do we ensure that our users implement the interface methods that the process manager expects?

In C++ and many other languages these problems are solved with *inheritance*, in which a *derived* or *child* class gets all the methods and data of a *base* or *parent* class, and then adds its own unique differences. Here, we will define an *abstract* base class called `Process` that inlcudes all of the methods that the manager expects. Crucially, we will declare the methods that inheriting classes must provide, by leaving those methods *unimplemented* in the base class.

For example, here is part of the `Process` class definition in `elma`.
```c++
  class Process {

      public:

      typedef enum { UNINITIALIZED, STOPPED, RUNNING } status_type;

      Process(std::string name) : _name(name), _status(UNINITIALIZED) {}
      virtual ~Process() = default;

      // Interface for derived classes
      virtual void init() = 0;
      virtual void start() = 0;
      virtual void update() = 0;
      virtual void stop() = 0;
      
      virtual string name() { return _name; }
      status_type status() { return _status; }

      private:

      string _name;
      status_type _status;

  };
```
The methods declared as `virtual` and set to `0` are called *pure virtual* functions and are not implemented by `Process`. Pure virtual methods must be implemented by child classes, unless they too are inteded to be virtual. If you try to construct an object of type `Process`, you will get a compiler error that looks something like:
```bash
error: invalid cast to abstract class type 'elma::Process'
```

This declaration of `Process` above also includes a *virtual* method `name`, which has an implementation, but can be overridden. Thus, virtual as opposed to pure virtual methods actually have implementations. Derived classes *may* override them, but do not have to.

Interestingly, derived classes can also override non-virtual methods. However, doing so will have consequences. Suppose we derive a class as follows.
```c++
class BoringProcess : public Process {
    void init() {}    // overriding pure virtual
    void start() {}
    void update() {}
    void stop() {}
    string name() { return "nothing"; }             // overiding a virtual
    status_type status() { return UNINITIALIZED; }  // overrinding a non-virtual
}
```
Now suppose we define two variables to refer to processs:
```c++
BoringProcess p("p");
Process * q = &p; // Ok to define virtual base class pointers
q->update();      // Calls BoringProcess.update()
q->name();        // Calls BoringProcess.name()
q->status();      // Calls Process.status() (probably not what we wanted)
```
To summarize:
- If you want to enforce derived classes to implement a method, use *pure virtual* (=0)
- If you want to make it optional for derived classes to override a method, use *virtual*.
- Avoid overriding non-virtual methods. 

Instance variables
===
Processes have a number of quantites to keep track of, such as their status, the time since they were last updated, and so on. We define the following private data members for `Process`:
```c++
private:

string _name;
status_type _status;
high_resolution_clock::duration _period,          // request time between updates
                                _previous_update, // duration from start to update before last
                                _last_update;     // duration from start to last update
time_point<high_resolution_clock> _start_time;    // time of most recent start
int _num_updates;                                 // number of times update() has been called
Manager * _manager_ptr;                           // a pointer to the manager
```
Variables that are specific to an instance of a class are called *instance variables*. Note that by convention we are prefixing all instance variables by an underscore "`_`", so it is easy to see which variables are which. Doing so also makes it less likely that derived classes will override them by accident (unless they use the same convention). We do not want any derived class to access these variables directly, so they are declared `private`. However, derived classes might want to know the value of these variables are, so we provide a public interface to all of them:
```c++
inline string name() { return _name; }
inline status_type status() { return _status; }
inline high_resolution_clock::duration period() { return _period; }
inline int num_updates() { return _num_updates; }
inline time_point<high_resolution_clock> start_time() { return _start_time; }
inline high_resolution_clock::duration last_update() { return _last_update; }
inline high_resolution_clock::duration previous_update() { return _previous_update; }
```
The keyword `inline` states that compiled code using these methods will simply replace method calls by the body of the function, so that no function calls will be made when they are used. Using `inline` is generally faster. If the function body gets big and is used frequently, `inline` takes up a lot of space in the compiled code, so `inline` is generally used only with very short function bodies. 

Process / Manager Interface
===

The process manager needs to update the timing information and status of a process as it manipulates it. Do enable this in such a way that our user does not need to think about it, we define a version of each of the main methods that are just for the manager to use. In particular, we add to our `Process` class:
```c++
class Manager; // declare that Manager is a class, 
               // defined elsewhere

class Process {

  ...

  friend Manager; // give Manager class access to 
                  // private methods and data

  ...

  private:
  // Manager interface
  void _init();
  void _start(high_resolution_clock::duration elapsed);
  void _update(high_resolution_clock::duration elapsed);
  void _stop();

  ...

};
```
The argument to `_start_` and `_update` is the duration of time since the manager was started. Now, the manager does not call the the user versions of these methods directly, but called the underscored private interface instead. These methods are implemented as follows. First, the `_init` method should change the status of the process and then call the user's `init` method:
```c++
void Process::_init() { 
    _status = STOPPED;     
    init();
}
```
The `_start` method sets the status of the process to `RUNNING`, and initializes the start time, last update time, and the number of updates. Then it calls the user's `start` method.
```c++
void Process::_start(high_resolution_clock::duration elapsed) { 
    _status = RUNNING; 
    _start_time = high_resolution_clock::now();
    _last_update = elapsed;
    _num_updates = 0;
    start();
}
```
The `_update` method updates the previous update and last update, calls the user's `update` method and then increments the number of udpates.
```c++
void Process::_update(high_resolution_clock::duration elapsed) {
    _previous_update = _last_update;
    _last_update = elapsed;
    update();
    _num_updates++;
}
```
Finally, the `_stop` method calls the 
```c++
void Process::_stop() { 
    _status = STOPPED;
    stop();
```

Other Process Methods
===

We define a couple of convenience methods for `Process` that allow the user to get at the underlying timing information as follows:
```c++
double Process::milli_time() {
    duration<double, std::milli> time = last_update();
    return time.count();
}
double Process::delta() { 
    duration<double, std::milli> diff = last_update() - previous_update();
    return diff.count();
}
```
The first method returns the number of milliseconds since the last update, and the second returns the difference between the last update and the previous update durations. Note that `_last_update` is updates just before the user's `update` method is called, so it should be approximately the current amount of time since the process started. 

The Manager
===
The manager is class is defined as follows:
```c++
class Manager {

    public: 

    Manager() {}

    Manager& schedule(Process& process, high_resolution_clock::duration period);

    Manager& all(std::function<void(Process&)> f);

    Manager& init();
    Manager& start();
    Manager& stop();
    Manager& run(high_resolution_clock::duration);
    Manager& update();

    inline high_resolution_clock::time_point start_time() { return _start_time; }
    inline high_resolution_clock::duration elapsed() { return _elapsed; }

    private:

    vector<Process *> _processes;
    high_resolution_clock::time_point _start_time;
    high_resolution_clock::duration _elapsed;

};
```
We go through these methods one by one. First, the Manager keeps a vector of process pointers, `_processes`. To add some processes, the user just does something like 
```c++
m.schedule(p, high_resolution_clock::duration(milliseconds(1)))
 .schedule(p, high_resolution_clock::duration(milliseconds(2)));
```
Note that the `schedule` method returns a reference to the manager itself so that you can do `method chaining` on the manager. This makes code a bit easier to read. The implementation of `schedule` is 
```c++
Manager& Manager::schedule(Process& process, high_resolution_clock::duration period) {

    process._period = period;
    process._manager_ptr = this;            
    _processes.push_back(&process); 
    return *this;

}
```
The manager accesses the processes' private data to set the period and manager pointer, adds the process to the list, and then returns a reference to itself for method chaining. 

The next method, `all` is a convenience method for running a function on all the processes in the list. It takes as an argument a `function` using the `std::functional` library. This allows you to send in either function pointers or lambdas. Its definition is
```c++
Manager& Manager::all(std::function< void(Process&) > f) {
    for(auto process_ptr : _processes) {
        f(*process_ptr);
    }
    return *this;
}
```
Using `all`, it is easy to start, stop, and init all of the process:
```c++
Manager& Manager::init() {
    return all([](Process& p) { p._init(); });
}

Manager& Manager::start() {
    return all([this](Process& p) { p._start(_elapsed); });
}    

Manager& Manager::stop() {
    return all([](Process& p) { p._stop(); });
}   
```
Note that since the `start` method needs to send the `elapsed` time to the processes' `_start` method, the lambda in the expression has to capture `this`. Also, since `all` returns `*this`, so do all of these methods that use `all`. 

The more interesting method is `update`, which should only update a process at the requested frequency.
```c++
Manager& Manager::update() { 
    return all([this](Process& p) {
        if ( _elapsed > p.last_update() + p.period() ) {
            p._update(_elapsed);
        }
    });
}
```
Finally, we define a `run` method that runs all processes for the requested amount of time
```c++
Manager& Manager::run(high_resolution_clock::duration runtime) {

    _start_time = high_resolution_clock::now();
    _elapsed = high_resolution_clock::duration::zero();

    start();        

    while ( _elapsed < runtime ) {
        update();
        _elapsed = high_resolution_clock::now() - _start_time;
        // TODO: sleep until next update, 
        // to give processor back to the OS
    }

    stop();

    return *this;

}
```
This `run` method is not ideal, because it does nothing but check the condition in the `while` loop and the condition in `update` over and over, only occasionally actually calling a processes' `_update` method. Future versions of `elma` will sleep until the next update. 

Here is an example that uses all of the methods we have defined so far. First, it is convenient to define a C preprocessor macro:
```c
#define MS(__ms__) high_resolution_clock::duration(milliseconds(__ms__))
```
which allows us to write `MS(10)` instead of `high_resolution_clock::duration(milliseconds(10))` over and over.

```c++
class MyProcess : public elma::Process {
  public:
    MyProcess(std::string name) : Process(name) {}
    void init() {}
    void start() {}
    void update() { 
      std::cout << name() << ": " 
                << milli_time()
                << "ms\n";
    }
    void stop() {}
};

TEST(Manager,Schedule) {

  elma::Manager m;
  MyProcess p("A"), q("B");

  m.schedule(p, MS(1))
    .schedule(q, MS(5))
    .init()
    .run(MS(11));

  ASSERT_EQ(p.num_updates(), 10);
  ASSERT_EQ(q.num_updates(), 2);

}
```
Running this in a test prints out
```
A: 1.0025ms
A: 2.0032ms
A: 3.0065ms
A: 4.0101ms
B: 5.0036ms
A: 5.0558ms
A: 6.0564ms
A: 7.0572ms
A: 8.0612ms
A: 9.0652ms
B: 10.0139ms
A: 10.0689ms
```
Note that the test may not always pass. This is because the operating system may lag for whatever reason, not giving the manager exclusive access to the CPU. In this sense, `elma` running on Linux is *not* a real time system, as it cannot guarantee the frequency with which a process will run.

Interprocess Communication
===
There are many ways that concurrent systems deal with process communication. Here are a few:
- **Shared variables:** in some global space that all processes have access to. The downside is the lack of enforceable conventions on how variables are accessed.
- **Events:** Processes can trigger and listen to events. Each event has associated data. Interrupts and interrupt handlers are an example. We will incorporate events into `elma` eventually.
- **Channels:** First in first out queues that let processes send data that other processes can subscribe to. This is common with embedded real time systems where, for example, a sensor is sending data continuously that other processes consume. 

We will start with channels. Here is a class definition.
```c++
class Channel {

    public:

    Channel(string name) : _name(name), _capacity(100) {}
    Channel(string name, int capacity) : _name(name), _capacity(capacity) {}

    Channel& send(double);
    Channel& flush(double);
    double latest();
    double earliest();

    inline int size() { return _queue.size(); }
    inline bool empty() { return _queue.size() == 0; }
    inline bool nonempty() { return _queue.size() > 0; }
    inline string name() { return _name; }
    inline int capacity() { return _capacity; }

    private:

    string _name;
    int _capacity;
    deque<double> _queue;

};
```
The main data structure is the double ended queue. We will push new data onto the front of the queue, and get the data off of the back of the queue. Most processes will likely just use the latest value, but some processes might want to know many values previous, so our channel records them, up to a limit.

The implementation of these methods is straightfoward, and in some ways just a wrapper around the `deque`. 
```c++
Channel& Channel::send(double value) {
    _queue.push_front(value);
    while ( _queue.size() >= capacity() ) {
        _queue.pop_back();
    }
    return *this;
}

Channel& Channel::flush(double) {
    _queue.clear();
    return *this;
}

double Channel::latest() {
    if ( _queue.size() == 0 ) {
        throw std::range_error("Tried to get the latest value in an empty channel.");
    }
    return _queue.front();
}

double Channel::earliest() {
    if ( _queue.size() == 0 ) {
        throw std::range_error("Tried to get the earliest value in an empty channel.");
    }
    return _queue.back();        
}  
```
Note that we throw exceptions if a user process tries to access an empty channel. 

To make channels accessible to the manager, we add a new private datum `_channels`:
```c++
map<string, Channel *> _channels;
```
and a method for adding and accessing them
```c++
Manager& Manager::add_channel(Channel& channel) {
    _channels[channel.name()] = &channel;
    return *this;
}

Channel& Manager::channel(string name) {
    if ( _channels.find(name) != _channels.end() ) {
      return *(_channels[name]);
    } else {
        throw std::domain_error("Tried to access an unregistered or non-existant channel.");
    }
}
```
To make channels accessible to processes, we basically just go through the `_manager_ptr` in the process:
```c++
Channel& Process::channel(string name) {
    return _manager_ptr->channel(name);
}
```

Example: Cruise Control
===

As an example, we will develop a simple cruise control system in which we model a car with one process and a control system with another. The underlying mathematics look like this. Let `v` be the velocity of a car, `m` its mass, `k` the coefficient of rolling friction, and `u` the force being applied by the engine on the car. Then a very simplified model of the car's velocity comes from Newton's second law (`f = ma`):
```
m dv/dt = k v - u
```
This is a continuous model, but our processes are discrete. We can discretize the model using the definition of derivative:
```
dv/dt = lim(h->0) (v(t) - v(t-h)) / h 
```
For `h` small, `dv/dt` is thus approximately
```
dv/dt = (v(t) - v(t-h)) / h
```
or 
```
v(t) = v(t-h) + h dv/dt 
     = v(t-h) + h (k v - u)
```
In our case, `h` is given by the difference between the last update and the previous update, which is available to user processes via the `delta()` function. Thus, we can write
```c++
class Car : public elma::Process {
  public:
    ControllableCar(std::string name) : Process(name) {}
    void init() {}
    void start() {
      velocity = 0;
      force = 0;
    }
    void update() {
      if ( channel("Throttle").nonempty() ) {
        force = channel("Throttle").latest();
      }
      velocity += ( delta() / 1000 ) * ( - k * velocity + force ) / m;
          velocity += ( delta() / 1000 ) * ( - k * velocity + force ) / m;
          channel("Velocity").send(velocity);
          std::cout << "t: "  << milli_time() << " ms\t" 
                    << " u: " << force        << " N\t"
                    << " v: " << velocity     << " m/s\n";  
    }
    void stop() {}
  private:
    double velocity;
    double force;
    const double k = 0.02;
    const double m = 1000;
}; 
```
A very simple controller uses proportional feedback. Basically, we set
```
u = Kp ( v - vdes )
```
where `Kp` is the proportional gain and `vdes` is the desired velocity. To make a control process, we write
```c++
class CruiseControl : public elma::Process {
  public:
    CruiseControl(std::string name) : Process(name) {}
    void init() {}
    void start() {
      speed = 0;
    }
    void update() {
      if ( channel("Velocity").nonempty() ) {
        speed = channel("Velocity").latest();
      }
      channel("Throttle").send(-KP*(speed - desired_speed));
    }
    void stop() {}
  private:
    double speed;
    const double desired_speed = 50.0,
                 KP = 0.5;
}; 
```
To set everything up, we add the two processes and the two channels to the manager, initialize everything, and run the system.
```c++
elma::Manager m;

Car car("Car");
CruiseControl cc("Control");
elma::Channel throttle("Throttle");
elma::Channel velocity("Velocity");

m.schedule(car, MS(10))
 .schedule(cc, MS(10))
 .add_channel(throttle)
 .add_channel(velocity)
 .init()
 .run(MS(10000));
```
Note that the order of the scheduling is important. We want the controller to run right after the car. If we reversed the order, the controller would be using an older sensor value. 

The output of the system for the first few steps looks like:
```
t: 10.0029 ms    u: 2.15007e-317 m^2/s   v: 2.15072e-319 m/s
t: 20.0034 ms    u: 25 m^2/s             v: 0.250013 m/s
t: 30.007 ms     u: 24.875 m^2/s         v: 0.498802 m/s
t: 40.0107 ms    u: 24.7506 m^2/s        v: 0.7463 m/s
t: 50.0109 ms    u: 24.6269 m^2/s        v: 0.992424 m/s
t: 60.0139 ms    u: 24.5038 m^2/s        v: 1.23734 m/s
t: 70.0157 ms    u: 24.3813 m^2/s        v: 1.48095 m/s
t: 80.0162 ms    u: 24.2595 m^2/s        v: 1.72326 m/s
t: 90.0168 ms    u: 24.1384 m^2/s        v: 1.96431 m/s
t: 100.018 ms    u: 24.0178 m^2/s        v: 2.20412 m/s
t: 110.019 ms    u: 23.8979 m^2/s        v: 2.44268 m/s
t: 120.019 ms    u: 23.7787 m^2/s        v: 2.67999 m/s
t: 130.02 ms     u: 23.66 m^2/s          v: 2.91608 m/s
t: 140.022 ms    u: 23.542 m^2/s         v: 3.15095 m/s
...
```

Documentation
===
Documentation on `elma` is still sparce, but I will fill it in asap. You can view the documentation by running
```bash
make docs 
```
and then opening `html/index.html` in your browser. 

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
    double get_nanoseconds();  // number of nanoseconds counted
    ```
    All `get_` methods should return values accurate to the nanosecond. Here is an example usage:
    ```c++
    #define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(1500))

    Stopwatch w; // should set the stopwatch to 0 seconds
    w.start(); 
    SLEEP;
    w.stop();    
    std::cout << w.get_seconds() << "\n"; // about 1.5
    SLEEP;
    std::cout << w.get_seconds() << "\n"; // still about 1.5
    w.start(); 
    SLEEP;
    w.stop();    
    std::cout << w.get_seconds() << "\n"; // about 3.0
    w.reset();
    std::cout << w.get_seconds() << "\n"; // 0.0
    ```
    To test your method, we will use assertions that test that `get_seconds` (for example) is approximately equal to the number of seconds that the stopwatch should have counted.
1. Overload the `latest` method for `Channel` to return the `n` most recent values in a `vector` of values. If there are fewer than `n` values, return the most that can be retrieved. The vector returned should be new, and not a reference to the channel's `deque`. 
1. Write a process called `Random` that sends random doubles between 0 and 1 (inclusive) to a channel called `link`. Write another process called `Filter` that reads from the `link` channel and computes a filtered version of the channel. The constructor to `Filter` should take an additional set of weights to apply. Suppose the latest `n` values retrieved from `link` are `v1` ... `vn` (with `v1` the latest value). Then the filter should compute
    ```
    w1 * v1 + ... + vn * vn
    ```
    Store the filtered value in an instance variable and make it available via a method `inline double value()`. The following code might be used to assemble the system:
    ```c++
    elma::Manager m;

    Random r("random numbers");
    Filter f("filter", { 0.5, 0.3, 0.2 });
    elma::Channel link("link");

    m.schedule(r, MS(10))
    .schedule(f, MS(10))
    .add_channel(link)
    .init()
    .run(MS(100));

    // Then assert something about f.value();
    ```
    By the way, you can generate raundom numbers using the [rand](http://www.cplusplus.com/reference/cstdlib/rand/) function availale in `stdlib`. You will need to convert the integer to a double and scale it using `RAND_MAX`. 
1. Read the documentation on the [tuple](http://www.cplusplus.com/reference/tuple/tuple/?kw=tuple) template in C++. The goal of this exercise to a method that lists information about the processes in the manager, much like the `ps waux` listing in UNIX. In particular, write a Manager method
    ```c++
    map<string, tuple<string, double, double, int>> ps();
    ```
    mapping a process name to its `_status`, its `_last_update` (in milliseconds), its latest value of `delta()` (in milliseconds), and the number of updates via `_last_update` it has had since starting. 
    For the car and cruise control example the results might look this look like:
    ```c++
    auto info = m.ps();
    std::get<0>(info["car"]); // "STOPPED"
    std::get<1>(info["car"]); // 1234.5
    std::get<2>(info["car"]); // 10.014
    std::get<3>(info["car"]); // 123
    ```