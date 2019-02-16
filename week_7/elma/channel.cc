#include <iostream>
#include <stdexcept>
#include "elma.h"

namespace elma {
    
    //! Send a value
    //! \param value The value to send into the channel
    //! \return A reference to the channel, for chaining
    Channel& Channel::send(double value) {
        _queue.push_front(value);
        while ( _queue.size() >= capacity() ) {
            _queue.pop_back();
        }
        return *this;
    }

    //! Clear the channel, deleting all values in it
    //! \return A reference to the channel, for chaining
    Channel& Channel::flush(double) {
        _queue.clear();
        return *this;
    }

    //! Get the newest value.
    //! Throws an error if the channel is empty.
    //! \return A reference to the channel, for chaining
    double Channel::latest() {
        if ( _queue.size() == 0 ) {
            throw std::range_error("Tried to get the latest value in an empty channel.");
        }
        return _queue.front();
    }

    //! Get the oldest value.
    //! Throws an error if the channel is empty.
    //! \return A reference to the channel, for chaining
    double Channel::earliest() {
        if ( _queue.size() == 0 ) {
            throw std::range_error("Tried to get the earliest value in an empty channel.");
        }
        return _queue.back();        
    }    

}