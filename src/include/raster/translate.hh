#ifndef RASTER_TRANSLATE_HH
#define RASTER_TRANSLATE_HH

#include "types.hh"

namespace raster {
    template <typename F, typename M>
    struct translation {
    private:
        F f;
        M m;
    public:
        template <typename T, typename U>
        translation(T &&f, U &&m) : f(std::forward<T>(f)), m(std::forward<U>(m)) {}

        template <typename T>
        auto get(T y, T x) const -> decltype(f(m, y, x)) {
            return f(m, y, x);
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
    translation<F,M> translated(F &&f, M &&m) {
        return translation<F,M>(std::forward<F>(f), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_TRANSLATE_HH */
