#ifndef RASTER_TYPES_OBJECT_HH
#define RASTER_TYPES_OBJECT_HH

/**
 * Provides an implementation of the Matrix concept for user-defined types.
 */

namespace raster {
    /**
     * Returns the row count of a matrix-like object.
     * 
     * @param obj A matrix-like object
     * @return An integral value containing the row count
     */
    template <typename T>
    auto rows (T &&obj) -> decltype(obj.rows()) {
        return obj.rows();
    }
    
    /**
     * Returns the column count of a matrix-like object.
     * 
     * @param obj A matrix-like object
     * @return An integral value containing the column count
     */
    template <typename T>
    auto columns (T &&obj) -> decltype(obj.columns()) {
        return obj.columns();
    }
    
    /**
     * Returns the value of the cell (y,x) within the given matrix-like object.
     * 
     * @param obj A matrix-like object
     * @param y The cell's row index
     * @param x The cell's column index
     * @return The value of the cell at position (y,x)
     */
    template <typename T, typename U>
    auto get (T &&obj, U y, U x) -> decltype(obj.get(y, x)) {
        return obj.get(y, x);
    }
    
    /**
     * Sets the value of the cell (y,x) within the given matrix-like object.
     * 
     * @param obj A matrix-like object
     * @param y The cell's row index
     * @param x The cell's column index
     * @param v The value to be assigned to the cell
     * @return The value to be assigned
     */
    template <typename T, typename U, typename V>
    auto set (T &&obj, U y, U x, V v) -> decltype(obj.set(y, x, v)) {
        return obj.set(y, x, v);
    }
}

#endif /* #ifndef RASTER_TYPES_OBJECT_HH */
