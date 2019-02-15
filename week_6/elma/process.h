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

     //! An abstract base class for processes
     /*!
     Derived classes should imlement the init, start, update, and stop methods.
     */
     class Process {

        public:

        friend class Manager;

        typedef enum { UNINITIALIZED, STOPPED, RUNNING } status_type;

        //! Default constructor. Names process "no name"
        Process() : _name("no name"), _status(UNINITIALIZED) {}

        //! Constructor that takes a name for the process
        /*!
          \param name The name of the process
        */
        Process(std::string name) : _name(name), _status(UNINITIALIZED) {}
        virtual ~Process() = default;

        // Interface for derived classes
        virtual void init() = 0;
        virtual void start() = 0;
        virtual void update() = 0;
        virtual void stop() = 0;

        // Inline methods that just replace method calls with their bodies
        inline string name() { return _name; }
        inline status_type status() { return _status; }
        inline high_resolution_clock::duration period() { return _period; }
        inline int num_updates() { return _num_updates; }
        inline time_point<high_resolution_clock> start_time() { return _start_time; }
        inline high_resolution_clock::duration last_update() { return _last_update; }
        inline high_resolution_clock::duration previous_update() { return _previous_update; }

        // Other getters
        Channel& channel(string name);
        double milli_time();
        double delta();

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