#ifndef RASTER_CONSTRAIN_HH
#define RASTER_CONSTRAIN_HH

/**
 * Coordinate-constraining access wrapper for matrices.
 */

#include <utility>
#include "../types.hh"
#include "../traits.hh"

namespace raster {
    /**
     * A coordinate constraining matrix wrapper.
     * 
     * This decorator class disables reads and writes outside the boundaries
     * of the wrapped matrix.
     * 
     * @tparam T The type of the wrapped matrix
     */
    template <typename T>
    class constrained {
        T matrix;
    public:
        /**
         * Creates a coordinate constraining wrapper around m
         * 
         * @param m The matrix to be wrapped
         */
        template <typename U>
        constrained(U &&m) : matrix(std::forward<U>(m)) {}
        
        /**
         * Returns the value of the cell (y,x) within this matrix.
         * 
         * If the cell coordinates lie outside the boundaries this matrix,
         * a default-constructed cell value is returned.
         * 
         * @param y The cell's row index
         * @param x The cell's column index
         * @return The value of the cell at position (y,x)
         */
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
        
        /**
         * Sets the value of the cell (y,x) within this matrix.
         * 
         * No assignment will be made If the cell coordinates lie outside the boundaries this matrix.
         * 
         * @param y The cell's row index
         * @param x The cell's column index
         * @param value The value to be assigned to the cell
         * @return value
         */
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
        
        /**
         * Returns the row count of this matrix.
         */
        auto rows() const -> decltype(rows(matrix)) {
            using raster::rows;
            return rows(matrix);
        }
        
        /**
         * Returns the column count of this matrix.
         */
        auto columns() const -> decltype(columns(matrix)) {
            using raster::columns;
            return columns(matrix);
        }
    };
    
    
    /**
     * Returns a coordinate-constraining wrapped around the given matrix.
     * 
     * @param m The matrix to be wrapped.
     * @return A coordinate constraining matrix wrapper around m
     */
    template <typename T>
    constrained<T> constrain(T &&m) {
        return constrained<T>(std::forward<T>(m));
    }
}

#endif /* #ifndef RASTER_CONSTRAIN_HH */
