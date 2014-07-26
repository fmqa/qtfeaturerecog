#ifndef RASTER_CONSTRAIN_HH
#define RASTER_CONSTRAIN_HH

#include <utility>
#include "../types.hh"
#include "../traits.hh"

namespace raster {
    template <typename T>
    class constrained {
        T matrix;
    public:
        template <typename U>
        constrained(U &&m) : matrix(std::forward<U>(m)) {}
        
        template <typename U>
        typename traits<T>::value_type get(U y, U x) const {
            using raster::rows;
            using raster::columns;
            using raster::get;
            if (y >= 0 && x >= 0 && y < rows(matrix) && x < columns(matrix)) {
                return get(matrix, y, x);
            }
            return typename traits<T>::value_type();
        }
        
        template <typename U, typename V>
        typename traits<T>::value_type set(U y, U x, V &&value) {
            using raster::rows;
            using raster::columns;
            using raster::set;
            if (y >= 0 && x >= 0 && y < rows(matrix) && x < columns(matrix)) {
                return set(matrix, y, x, value);
            }
            return typename traits<T>::value_type();
        }
        
        auto rows() const -> decltype(rows(matrix)) {
            using raster::rows;
            return rows(matrix);
        }
        
        auto columns() const -> decltype(columns(matrix)) {
            using raster::columns;
            return columns(matrix);
        }
    };
    
    template <typename T>
    constrained<T> constrain(T &&m) {
        return constrained<T>(std::forward<T>(m));
    }
}

#endif /* #ifndef RASTER_CONSTRAIN_HH */
