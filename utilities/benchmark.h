#ifndef __BENCHMARK__
#define __BENCHMARK__

#include "spatial/spatial.h"
#include "utilities.h"


namespace benchmark {
    template<typename T>
    void nearest_one(Spatial<T> & spatial, const T & ref, unsigned int repeat = 1);

    template<typename T>
    void nearest_many(Spatial<T> & spatial, const T & ref, unsigned int count, unsigned int repeat = 1);

    template<typename T>
    void radius(Spatial<T> & spatial, const T & ref, real radius, unsigned int repeat = 1);
}

#endif // __BENCHMARK__
