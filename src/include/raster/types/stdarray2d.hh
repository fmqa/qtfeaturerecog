#ifndef RASTER_TYPES_STDARRAY2D_HH
#define RASTER_TYPES_STDARRAY2D_HH

#include <array>

namespace raster {
    template <typename T, size_t M, size_t N>
    size_t rows (const std::array<std::array<T,N>,M> &) {
        return M;
    }
    
    template <typename T, size_t M, size_t N>
    size_t columns (const std::array<std::array<T,N>,M> &) {
        return N;
    }
    
    template <typename T, typename U, size_t M, size_t N>
    T get (const std::array<std::array<T,N>,M> &array2d, U y, U x) {
        return array2d[y][x];
    }
    
    template <typename T, typename U, typename V, size_t M, size_t N>
    T set (std::array<std::array<T,N>,M> &array2d, U y, U x, V v) {
        return array2d[y][x] = v;
    }
}

#endif /* #ifndef RASTER_TYPES_STDARRAY2D_HH */
