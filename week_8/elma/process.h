#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>
#include <functional>

#include "elma.h"

namespace elma {

    class Manager;
    class Channel;

    using std::string;
    using namespace std::chrono;

     //! An abstract base class for processes

     //! Derived classes should imlement the init, start, update, and stop methods.
     //! For example, here is a process that prints out a message every time its
     //! update method is called.
     //! \include examples/basic.cc
     class Process {

        public:

        friend class Manager;

        //! Status of the process, as managed by a Manager. Get the
        //! status using the status() getter.
        typedef enum { UNINITIALIZED, STOPPED, RUNNING } status_type;

        //! Default constructor. Names process "no name"
        Process() : _name("unnamed process"), _status(UNINITIALIZED), _manager_ptr(NULL) {}

        //! Constructor that takes a name for the process
        /*!
          \param name The name of the process
        */
        Process(std::string name) : _name(name), _status(UNINITIALIZED), _manager_ptr(NULL) {}
        virtual ~Process() = default;

        // Interface for derived classes

        //! Initialization method. This method should be overridden by derived
        //! classes. It will usually be called once, after all processes and
        //! communication objects have been added to the manager, but before
        //! the Manager starts running.
        virtual void init() = 0;

        //! Start method. This method should be  overridden by derived
        //! classes. It will be called just before the manager starts running.
        //! It may be called multiple times, if the manager is started and stopped.
        virtual void start() = 0;

        //! Update method. This method should be  overridden by derived
        //! classes. It will be called repeatedly by the manager at a frequency
        //! determined by the period used when the process is scheduled with the
        //! Manager (see Manager::schedule).
        virtual void update() = 0;

        //! Stop method. This method should be  overridden by derived
        //! classes. It will be called just after the manager stops running.
        //! It may be called multiple times, if the manager is started and stopped.        
        virtual void stop() = 0;

        // Inline methods that just replace method calls with their bodies

        //! Getter
        //! \return The name of the process.
        inline string name() { return _name; }

        //! Getter
        //! \return The status of the process
        inline status_type status() { return _status; }

        //! Getter
        //! \return The period of the process.
        inline high_resolution_clock::duration period() { return _period; }

        //! Getter
        //! \return The number of updates since the process was last started by the Manager.
        inline int num_updates() { return _num_updates; }

        //! Getter
        //! \return The time the process was last started by the Manager.        
        inline time_point<high_resolution_clock> start_time() { return _start_time; }

        //! Getter
        //! \return The duration of time between the start time and the most recent 
        //! time the Manager called the update() method.   
        inline high_resolution_clock::duration last_update() { return _last_update; }

        //! Getter
        //! \return The duration of time between the start time and the second most recent 
        //! time the Manager called the update() method.        
        inline high_resolution_clock::duration previous_update() { return _previous_update; }

        // documentation for these methods is in process.cc
        Channel& channel(string name);
        double milli_time();
        double delta();

        void watch(string event_name, std::function<void(Event&)> handler);
        void emit(const Event& event);

        void http_get(std::string url, std::function<void(json&)> handler);

        private:

        // Manager interface
        void _init();
        void _start(high_resolution_clock::duration elapsed);
        void _update(high_resolution_clock::duration elapsed);
        void _stop();

        // Instance variables
        string _name;
        status_type _status;
        high_resolution_clock::duration _period,          // request time between updates
                                        _previous_update, // duration from start to update before last
                                        _last_update;     // duration from start to last update
        time_point<high_resolution_clock> _start_time;    // time of most recent start
        int _num_updates;                                 // number of times update() has been called
        Manager * _manager_ptr;                           // a pointer to the manager        

    };

}

#endif 