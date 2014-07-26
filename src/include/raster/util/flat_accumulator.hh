#ifndef RASTER_FLAT_ACCUMULATOR_HH
#define RASTER_FLAT_ACCUMULATOR_HH

#include <utility>
#include <functional>
#include "accumulator.hh"
#include "constrain.hh"
#include "view2d.hh"

namespace raster {
    template <typename T, typename U>
    using constrained_accumulator = 
        raster::accumulative_iterator<
            raster::constrained<T>,
            decltype(std::mem_fn(&U::first)),
            decltype(std::mem_fn(&U::second))
        >;
    
        
    template <typename T, typename U>
    using constrained_flat_accumulator = constrained_accumulator<view2d<T,U>,std::pair<U,U>>;
    
    template <typename T, typename U>
    constrained_flat_accumulator<T,U> as_constrained_flat_acc(U height, U width, T &x) {
        return raster::accumulator(raster::constrain(raster::as2d(height, width, x)),
                                   std::mem_fn(&std::pair<U,U>::first),
                                   std::mem_fn(&std::pair<U,U>::second));
    }
}

#endif /* #ifndef RASTER_FLAT_ACCUMULATOR_HH */
