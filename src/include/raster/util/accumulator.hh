#ifndef RASTER_ACCUMULATOR_HH
#define RASTER_ACCUMULATOR_HH

/**
 * Output iterator facade for accumulator matrices.
 */

#include "../types.hh"

namespace raster {
    /**
     * Wraps an accumulator matrix in an iterator interface.
     * 
     * @tparam M The accumulator matrix's type
     * @tparam X x-coordinate accessor function type for input values
     * @tparam Y y-coordinate accessor function type for input values
     */
    template <typename M, typename X, typename Y>
    class accumulative_iterator : public std::iterator<std::output_iterator_tag,void,void,void,void> {
        M matrix;
        X x;
        Y y;
    public:
        /**
         * Creates an accumulative iterator wrapping the given matrix.
         * 
         * @param m The accumulator matrix to be wrapped
         * @param x The x-coordinate accessor function
         * @param y The y-coordinate accessor function
         */
        template <typename T, typename U, typename V>
        accumulative_iterator(T &&m, U &&x, V &&y) 
        : matrix(std::forward<T>(m)),
          x(std::forward<U>(x)),
          y(std::forward<U>(y))
          {}
        
        /**
         * Increments the value of the cell at the coordinates (y(p),x(p)) by 1.
         * 
         * @param p An object containing the coordinates of a matrix cell.
         * @return A reference to this iterator
         */
        template <typename T>
        accumulative_iterator& operator=(const T &p) {
            set(matrix, y(p), x(p), get(matrix, y(p), x(p)) + 1);
            return *this;
        }
        
        accumulative_iterator& operator*() { return *this; }
        accumulative_iterator& operator++() { return *this; }
        accumulative_iterator& operator++(int) { return *this; }
    };
    
    /**
     * Returns an accumulative iterator wrapping the given matrix.
     * 
     * @param m The matrix to be wrapped.
     * @param x The x-coordinate accessor function for input objects.
     * @param y The y-coordinate accessor function for input objects.
     * @return An accumulative iterator wrapping m
     */
    template <typename M, typename X, typename Y>
    accumulative_iterator<M,X,Y> accumulator(M &&m, X &&x, Y &&y) {
        return accumulative_iterator<M,X,Y>(std::forward<M>(m),
                                            std::forward<X>(x),
                                            std::forward<Y>(y));
    }
}

#endif /* #ifndef RASTER_ACCUMULATOR_HH */
