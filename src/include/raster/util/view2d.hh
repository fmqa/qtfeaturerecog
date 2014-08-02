#ifndef RASTER_VIEW2D_HH
#define RASTER_VIEW2D_HH

/**
 * Matrix wrapper for one-dimensional vectors.
 */

namespace raster {
    /**
     * A matrix view of a one-dimensional vector.
     * 
     * @tparam T The vector type
     * @tparam S The size type of the vector
     */
    template <typename T, typename S>
    class view2d {
        T *vector;
        S height;
        S width;
    public:
        view2d(S rows, S columns, T &v) 
        : vector(&v), 
          height(rows), 
          width(columns) {}
          
        view2d()
        : vector(),
          height(),
          width() {}
        
        template <typename U>
        auto get(U y, U x) const -> decltype((*vector)[y * width + x]) {
            return (*vector)[y * width + x];
        }
        
        template <typename U, typename V>
        V set(U y, U x, V value) const {
            return (*vector)[y * width + x] = value;
        }
        
        S rows() const { return height; }
        S columns() const { return width; }
    };
    
    /**
     * Returns a matrix view of the given vector-like object.
     * 
     * @param rows The row count of the matrix view
     * @param column The column count of the matrix view
     * @param vector The underlying vector containing the cell data
     * @return A matrix view of the given vector
     */
    template <typename T, typename S>
    view2d<T,S> as2d(S rows, S columns, T &vector) {
        return view2d<T,S>(rows, columns, vector);
    }
    
    /**
     * Returns a read-only matrix view of the given vector-like object.
     * 
     * @param rows The row count of the matrix view
     * @param column The column count of the matrix view
     * @param vector The underlying vector containing the cell data
     * @return A matrix view of the given vector
     */
    template <typename T, typename S>
    view2d<const T,S> asconst2d(S rows, S columns, const T &vector) {
        return view2d<const T,S>(rows, columns, vector);
    }
}

#endif /* #ifndef RASTER_VIEW2D_HH */
