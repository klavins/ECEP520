#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <deque>
#include <json/json.h>

#include "elma.h"

namespace elma {

    using std::string;
    using std::deque;
    using nlohmann::json; 

    //! A channel for sending double values to and from Process objects

    //! Here is an example that uses channels that enables a model of a car and
    //! a simple cruise controller to communicate.
    //! \include examples/feedback.cc
    class Channel {

        public:

        //! Constructor
        //! \param name The name of the channel
        Channel(string name) : _name(name), _capacity(100) {}

        //! Constructor
        //! \param name The name of the channel
        //! \param capacity The maximum number of values to store in the channel
        Channel(string name, int capacity) : _name(name), _capacity(capacity) {}

        Channel& send(json);
        Channel& flush();
        json latest();
        json earliest();

        //! Getter
        //! \return The number of values in the channel
        inline int size() { return _queue.size(); }

        //! Getter
        //! \return Whether the channel is empty      
        inline bool empty() { return _queue.size() == 0; }

        //! Getter
        //! \return Whether the channel is not empty
        inline bool nonempty() { return _queue.size() > 0; }

        //! Getter
        //! \return The name of the channel      
        inline string name() { return _name; }

        //! Getter
        //! \return The capacity of the channel      
        inline int capacity() { return _capacity; }

        private:

        string _name;
        int _capacity;
        deque<json> _queue;

    };

}

#endif