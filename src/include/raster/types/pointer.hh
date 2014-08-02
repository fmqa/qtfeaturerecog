#ifndef RASTER_TYPES_POINTER_HH
#define RASTER_TYPES_POINTER_HH

/**
 * Provides an implementation of the Matrix concept for objects referred to by a pointer.
 */

namespace raster {
    template <typename T>
    auto rows(const T *ptr) -> decltype(rows(*ptr)) {
        return rows(*ptr);
    }
    
    template <typename T>
    auto columns(const T *ptr) -> decltype(columns(*ptr)) {
        return columns(*ptr);
    }
    
    template <typename T, typename U>
    auto get(const T *ptr, U y, U x) -> decltype(get(*ptr, y, x)) {
        return get(*ptr, y, x);
    }
    
    template <typename T, typename U, typename V>
    auto set(T *ptr, U y, U x, V v) -> decltype(set(*ptr, y, x, v)) {
        return set(*ptr, y, x, v);
    }
}

#endif /* #ifndef RASTER_TYPES_POINTER_HH */
