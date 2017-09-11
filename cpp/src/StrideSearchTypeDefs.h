#ifndef _STRIDE_SEARCH_TYPE_DEFS_H_
#define _STRIDE_SEARCH_TYPE_DEFS_H_

#include "StrideSearchConfig.h"
#include <vector>
#include <chrono>

#ifdef USE_NANOFLANN
#include "nanoflann.hpp"
#endif

namespace StrideSearch {

    /// Real number type
    typedef double scalar_type;
    
    /// Memory index type
    typedef int index_type;

    /// Latitude-Longitude coordinate pair type
    typedef std::pair<scalar_type, scalar_type> ll_coord_type;

    /// Vector of index_type type
    typedef std::vector<index_type> vec_indices_type;
    
    constexpr auto year_sec = 31556952ll; // seconds in average Gregorian year
    constexpr auto day_sec = 86400ll; // seconds in sidereal day

    typedef std::chrono::system_clock clock_type;
    typedef std::chrono::time_point<clock_type> time_point_type;
    typedef std::chrono::hours hour_type;
    typedef std::chrono::duration<index_type, std::ratio<year_sec>> year_type;
    typedef std::chrono::duration<scalar_type, std::ratio<day_sec>> day_type;
    
#ifdef USE_NANOFLANN

#endif
}

#endif
