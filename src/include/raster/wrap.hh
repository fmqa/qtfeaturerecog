#ifndef RASTER_WRAP_HH
#define RASTER_WRAP_HH

#include "translate.hh"

namespace raster {
    struct extender {
        template <typename M, typename T, typename U>
        auto operator()(M &&m, T y, U x) const -> decltype(get(m, y, x)) {
            return get(m, 
                       y < 0 ? 0 : y >= rows(m) ? rows(m) - 1 : y,
                       x < 0 ? 0 : x >= columns(m) ? columns(m) - 1 : x);
        }
    };
    
    template <typename M>
    translation<extender,M> extend(M &&m) {
        return translation<extender,M>(extender(), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_WRAP_HH */
