#ifndef _LITERALS_H
#define _LITERALS_H

#include <chrono>

using namespace std::chrono;

//! \file

namespace elma {

    constexpr high_resolution_clock::duration operator"" _s ( unsigned long long x ) {
        return high_resolution_clock::duration(seconds(x));
    }        

    constexpr high_resolution_clock::duration operator"" _ms ( unsigned long long x ) {
        return high_resolution_clock::duration(milliseconds(x));
    }

    constexpr high_resolution_clock::duration operator"" _us ( unsigned long long x ) {
        return high_resolution_clock::duration(microseconds(x));
    }    

}

#endif