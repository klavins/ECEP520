#include <iostream>
#include <stdexcept>
#include "elma.h"

namespace elma {
    
    Channel& Channel::send(double value) {
        _queue.push_front(value);
        while ( _queue.size() > capacity() ) {
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

}