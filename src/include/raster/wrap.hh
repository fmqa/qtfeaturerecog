#ifndef RASTER_WRAP_HH
#define RASTER_WRAP_HH

/**
 * Provides edge-wrapping algorithms for matrices.
 */

#include "translate.hh"

namespace raster {
    /**
     * Edge-extending access wrapper.
     * 
     * This functor clamps the input coordinates to the edge coordinates of the given matrix.
     */ 
    struct extender {
        /**
         * Returns the value of the cell at position (y,x) within the matrix m.
         * 
         * If the position lies outside the bounds of the matrix, it will be clamped
         * to the edges of the matrix.
         * 
         * @param m The matrix to be accessed
         * @param y The cell's row index
         * @param x The cell's column index
         * @return The value of the cell to be accessed
         */
        template <typename M, typename T, typename U>
        auto operator()(M &&m, T y, U x) const -> decltype(get(m, y, x)) {
            return get(m, 
                       y < 0 ? 0 : y >= rows(m) ? rows(m) - 1 : y,
                       x < 0 ? 0 : x >= columns(m) ? columns(m) - 1 : x);
        }
    };
    
    /**
     * Returns a matrix wrapping the cell access of the given matrix in an
     * edge-extending fashion.
     * 
     * @param m The matrix to be wrapped
     * @return An edge-extending access wrapper around m
     */
    template <typename M>
    translation<extender,M> extend(M &&m) {
        return translation<extender,M>(extender(), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_WRAP_HH */
