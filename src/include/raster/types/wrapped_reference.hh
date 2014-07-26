#ifndef RASTER_TYPES_WRAPPED_REFERENCE_HH
#define RASTER_TYPES_WRAPPED_REFERENCE_HH

#include <functional>

namespace raster {
    template <typename T>
    auto rows(const std::reference_wrapper<T> &ref) -> decltype(rows(ref.get())) {
        return rows(ref.get());
    }
    
    template <typename T>
    auto columns(const std::reference_wrapper<T> &ref) -> decltype(columns(ref.get())) {
        return columns(ref.get());
    }
    
    template <typename T, typename U>
    auto get(const std::reference_wrapper<T> &ref, U y, U x) -> decltype(get(ref.get(), y, x)) {
        return get(ref.get(), y, x);
    }
    
    template <typename T, typename U, typename V>
    auto set(const std::reference_wrapper<T> &ref, U y, U x, V v) -> decltype(set(ref.get(), y, x, v)) {
        return set(ref.get(), y, x, v);
    }
}

#endif /* #ifndef RASTER_TYPES_WRAPPED_REFERENCE_HH */
