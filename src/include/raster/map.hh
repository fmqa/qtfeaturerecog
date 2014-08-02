#ifndef RASTER_MAP_HH
#define RASTER_MAP_HH

/**
 * Provides mapping algorithms for matrices.
 */

#include <utility>
#include "types.hh"

namespace raster {
    /**
     * Represents a matrix resulting from applying a function to each cell
     * of a matrix.
     * 
     * @tparam F The mapping function's type
     * @tparam M The matrix type
     */
    template <typename F, typename M>
    struct mapping {
    private:
        F f;
        M m;
    public:
        /**
         * Constructs a mapping matrix mapping the given matrix to the given
         * function.
         * 
         * @param f The mapping function
         * @param m The matrix to be mapped to f
         */
        template <typename T, typename U>
        mapping(T &&f, U &&m) : f(std::forward<T>(f)), m(std::forward<U>(m)) {}

        /**
         * Returns the value of the cell at the given coordinates in this matrix.
         * 
         * @param y The cell's row index
         * @param x The cell's column index
         * @return The value of the cell at the position (y,x)
         */
        template <typename T>
        auto get(T y, T x) const -> decltype(f(get(m, y, x))) {
            using raster::get;
            return f(get(m, y, x));
        }
        
        /**
         * Returns the number of rows within this matrix.
         */
        auto rows() const -> decltype(rows(m))  {
            using raster::rows;
            return rows(m);
        }
        
        /**
         * Returns the number of columns within this matrix.
         */
        auto columns() const -> decltype(columns(m)) {
            using raster::columns;
            return columns(m);
        }
    };
    
    /**
     * Constructs a mapping matrix from the given matrix and mapping function.
     * 
     * @param f The mapping function
     * @param m The matrix to be mapped to f
     * @return A matrix mapping cell values of m to f
     */
    template <typename F, typename M>
    mapping<F,M> mapped(F &&f, M &&m) {
        return mapping<F,M>(std::forward<F>(f), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_MAP_HH */

