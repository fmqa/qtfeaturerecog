#ifndef RASTER_TYPES_OBJECT_HH
#define RASTER_TYPES_OBJECT_HH

namespace raster {
    template <typename T>
    auto rows (T &&obj) -> decltype(obj.rows()) {
        return obj.rows();
    }
    
    template <typename T>
    auto columns (T &&obj) -> decltype(obj.columns()) {
        return obj.columns();
    }
    
    template <typename T, typename U>
    auto get (T &&obj, U y, U x) -> decltype(obj.get(y, x)) {
        return obj.get(y, x);
    }

    template <typename T, typename U, typename V>
    auto set (T &&obj, U y, U x, V v) -> decltype(obj.set(y, x, v)) {
        return obj.set(y, x, v);
    }
}

#endif /* #ifndef RASTER_TYPES_OBJECT_HH */
