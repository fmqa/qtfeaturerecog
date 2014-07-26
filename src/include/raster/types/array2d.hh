#ifndef RASTER_TYPES_ARRAY2D_HH
#define RASTER_TYPES_ARRAY2D_HH

#include <cstddef>

namespace raster {
    template <typename T, size_t M, size_t N>
    size_t rows (const T (&)[M][N]) {
        return M;
    }
    
    template <typename T, size_t M, size_t N>
    size_t columns (const T (&)[M][N]) {
        return N;
    }
        
    template <typename T, typename U, size_t M, size_t N>
    T get (const T (&array2d)[M][N], U y, U x) {
        return array2d[y][x];
    }
    
    template <typename T, typename U, typename V, size_t M, size_t N>
    T set (T (&array2d)[M][N], U y, U x, V v) {
        return array2d[y][x] = v;
    }
}

#endif /* #ifndef RASTER_TYPES_ARRAY2D_HH */
