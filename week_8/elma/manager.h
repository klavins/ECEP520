#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <map>
#include <chrono>
#include <functional>

#include "elma.h"

namespace elma {

    using std::string;
    using std::vector;
    using std::map;
    using namespace std::chrono;

    class Channel;
    class Process;

    //! The Process Manager class. 

    //! Example usage:
    //! \include examples/basic.cc

    class Manager {

        public: 

        //! Default constructor
        Manager() {}
        
        Manager& schedule(Process& process, high_resolution_clock::duration period);
        Manager& all(std::function<void(Process&)> f);

        Manager& init();
        Manager& start();
        Manager& update();        
        Manager& stop();

        Manager& run(high_resolution_clock::duration);

        //! Getter
        //! \return The time the Manager was most recently started
        inline high_resolution_clock::time_point start_time() { return _start_time; }

        //! Getter
        //! \return The duration of time since the manager was most recently started
        inline high_resolution_clock::duration elapsed() { return _elapsed; }

        // Channel Interface
        Manager& add_channel(Channel&);
        Channel& channel(string);

        // Event Interface
        Manager& watch(string event_name, std::function<void(Event&)> handler);
        Manager& emit(const Event& event);
        Client& client() { return _client; }

        private:
        vector<Process *> _processes;
        map<string, Channel *> _channels;
        map<string, vector<std::function<void(Event&)>>> event_handlers;
        high_resolution_clock::time_point _start_time;
        high_resolution_clock::duration _elapsed;
        Client _client;

    };

}

#endif