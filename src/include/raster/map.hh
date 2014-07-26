#ifndef RASTER_MAP_HH
#define RASTER_MAP_HH

#include <utility>
#include "types.hh"

namespace raster {
    template <typename F, typename M>
    struct mapping {
    private:
        F f;
        M m;
    public:
        template <typename T, typename U>
        mapping(T &&f, U &&m) : f(std::forward<T>(f)), m(std::forward<U>(m)) {}

        template <typename T>
        auto get(T y, T x) const -> decltype(f(get(m, y, x))) {
            using raster::get;
            return f(get(m, y, x));
        }
        
        auto rows() const -> decltype(rows(m))  {
            using raster::rows;
            return rows(m);
        }
        
        auto columns() const -> decltype(columns(m)) {
            using raster::columns;
            return columns(m);
        }
    };
    
    template <typename F, typename M>
    mapping<F,M> mapped(F &&f, M &&m) {
        return mapping<F,M>(std::forward<F>(f), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_MAP_HH */

