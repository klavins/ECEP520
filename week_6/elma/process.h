#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>

#include "elma.h"

namespace elma {

    class Manager;
    class Channel;

    using std::string;
    using namespace std::chrono;

    class Process {

    public:

    friend class Manager;

    typedef enum { UNINITIALIZED, STOPPED, RUNNING } status_type;

    Process(std::string name) : _name(name), _status(UNINITIALIZED) {}
    virtual ~Process() = default;

    // Interface for derived classes
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void stop() = 0;

    // Getters
    inline string name() { return _name; }
    inline status_type status() { return _status; }
    inline high_resolution_clock::duration period() { return _period; }

    inline int num_updates() { return _num_updates; }
    inline time_point<high_resolution_clock> start_time() { return _start_time; }
    inline high_resolution_clock::duration last_update() { return _last_update; }
    inline double milli_time() {
        duration<double, std::milli> time = last_update();
        return time.count();
    }
    inline high_resolution_clock::duration previous_update() { return _previous_update; }
    Channel& channel(string name);
    double delta() { 
        duration<double, std::milli> diff = last_update() - previous_update();
        return diff.count();
    }

    private:

    // Manager interface
    void _init() { 
        _status = STOPPED;     
        init();
    }
    void _start(high_resolution_clock::duration elapsed) { 
        _status = RUNNING; 
        _start_time = high_resolution_clock::now();
        _last_update = elapsed;
        _num_updates = 0;
        start();
    }
    void _update(high_resolution_clock::duration elapsed) {
        _previous_update = _last_update;
        _last_update = elapsed;
        update();
        _num_updates++;
    }
    void _stop() { 
        _status = STOPPED; 
        stop();
    }    

    string _name;
    status_type _status;
    high_resolution_clock::duration _period,
                                    _previous_update,
                                    _last_update;
    time_point<high_resolution_clock> _start_time;
    int _num_updates;
    Manager * _manager_ptr;          

    };

}

#endif 