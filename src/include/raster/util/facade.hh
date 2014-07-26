#ifndef RASTER_FACADE_HH
#define RASTER_FACADE_HH

#include <utility>
#include "../types.hh"

namespace raster {
    template <typename M, typename Derived>
    class facade {
        M m_mat;
    public:
        template <typename T>
        facade(T &&m) : m_mat(std::forward<T>(m)) {}
        
        template <typename T>
        auto get(T y, T x) const -> decltype(get(m_mat, y, x)) {
            using raster::get;
            static_cast<const Derived*>(this)->read(y, x);
            return get(m_mat, y, x);
        }
        
        template <typename T, typename V>
        auto set(T y, T x, V value) -> decltype(set(m_mat, y, x, value)) {
            using raster::set;
            static_cast<Derived*>(this)->write(y, x);
            return set(m_mat, y, x, value);
        }
        
        auto rows() const -> decltype(rows(m_mat)) {
            using raster::rows;
            return rows(m_mat);
        }
        
        auto columns() const -> decltype(columns(m_mat)) {
            using raster::columns;
            return columns(m_mat);
        }
    };
}

#endif /* #ifndef RASTER_FACADE_HH */
