Elma is an event loop and process manager for embedded systems. It keeps track of processes, executes them at specified frequencies, and enables interprocess communication.

Example
---

As an example, we will develop a simple cruise control system in which we model a car with one process and a control system with another. The underlying mathematics look like this. Let `v` be the velocity of a car, `m` its mass, `k` the coefficient of rolling friction, and `u` the force being applied by the engine on the car. Then a very simplified model of the car's velocity comes from Newton's second law (`f = ma`):

    m dv/dt = -k v + u

This is a continuous model, but our processes are discrete. We can discretize the model using the definition of derivative:

    dv/dt = lim(h->0) (v(t) - v(t-h)) / h 

For `h` small, `dv/dt` is thus approximately

    dv/dt = (v(t) - v(t-h)) / h

or 

    v(t) = v(t-h) + h dv/dt 
         = v(t-h) - h (k v - u)

In our case, `h` is given by the difference between the last update and the previous update, which is available to user processes via the `delta()` function. Thus, we can write

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

A very simple controller uses proportional feedback. Basically, we set

    u = - Kp ( v - vdes )

where `Kp` is the proportional gain and `vdes` is the desired velocity. To make a control process, we write

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
                    KP = 250.0;
    }; 

To set everything up, we add the two processes and the two channels to the manager, initialize everything, and run the system.

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

Note that the order of the scheduling is important. We want the controller to run right after the car. If we reversed the order, the controller would be using an older sensor value. 

The output of the system for the first few steps looks like:

