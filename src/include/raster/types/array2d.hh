#ifndef RASTER_TYPES_ARRAY2D_HH
#define RASTER_TYPES_ARRAY2D_HH

/**
 * Provides an implementation of the Matrix concept for C-Style 2D arrays
 */

#include <cstddef>

namespace raster {
    /**
     * Returns the number of rows (1st Dimension) in the given 2D array.
     */
    template <typename T, size_t M, size_t N>
    size_t rows (const T (&)[M][N]) {
        return M;
    }
    
    /**
     * Returns the number of columns (2nd Dimension) in the given 2D array.
     */
    template <typename T, size_t M, size_t N>
    size_t columns (const T (&)[M][N]) {
        return N;
    }
       
    /**
     * Returns the value of the cell (y,x) in the given row-major matrix.
     * 
     * @param array2d A matrix, represented as an array of row vectors
     * @param y the cell's row index
     * @param x the cell's column index
     * @return The value of the cell at position (y,x)
     */
    template <typename T, typename U, size_t M, size_t N>
    T get (const T (&array2d)[M][N], U y, U x) {
        return array2d[y][x];
    }
    
    /**
     * Sets the value of the cell (y,x) in the given row-major matrix.
     * 
     * @param array2d A matrix, represented as an array of row vectors
     * @param y The cell's row index
     * @param x The cell's column index
     * @param v The value to be assigned to the cell
     * @return The assigned value
     */
    template <typename T, typename U, typename V, size_t M, size_t N>
    T set (T (&array2d)[M][N], U y, U x, V v) {
        return array2d[y][x] = v;
    }
}

#endif /* #ifndef RASTER_TYPES_ARRAY2D_HH */
