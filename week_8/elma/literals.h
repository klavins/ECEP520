#ifndef _LITERALS_H
#define _LITERALS_H

#include <chrono>

using namespace std::chrono;

//! \file

namespace elma {

    //! A duration in seconds. For example 10_s. 
    constexpr high_resolution_clock::duration operator"" _s ( unsigned long long x ) {
        return high_resolution_clock::duration(seconds(x));
    }        

    //! A duration in milliseconds. For example 10_ms. 
    constexpr high_resolution_clock::duration operator"" _ms ( unsigned long long x ) {
        return high_resolution_clock::duration(milliseconds(x));
    }

    //! A duration in microseconds. For example 10_us. 
    constexpr high_resolution_clock::duration operator"" _us ( unsigned long long x ) {
        return high_resolution_clock::duration(microseconds(x));
    }    

}

#endif