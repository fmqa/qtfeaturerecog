#ifndef RASTER_CONV_HH
#define RASTER_CONV_HH

/**
 * Convolution algorithm for Matrices.
 */

#include "traits.hh"

namespace raster {
    /**
     * Specifies the type resulting from a convolution of 2 matrices.
     */
    template <typename T, typename U>
    using result = decltype(
        std::declval<typename traits<T>::value_type>() *
        std::declval<typename traits<U>::value_type>()
    );
    
    /**
     * Returns the convolution result of a kernel matrix with an image matrix 
     * at the given coordinates.
     * 
     * @param k A matrix-like object representing the kernel
     * @param f A matrix-like object representing the matrix to be convolved
     * @param y The row index of the convolution's center
     * @param x The column index of the convolution's center
     * @return The result of the convolution between k and f at the coordinates (y,x)
     */
    template <typename K, typename F, typename T>
    auto conv(K &&k, F &&f, T y, T x) -> result<K,F> {
        result<K,F> sum{};
        
        for (decltype(rows(k)) i = 0; i < rows(k); ++i) {
            for (decltype(columns(k)) j = 0; j < columns(k); ++j) {
                sum += get(k, i, j) * get(f, y + i - rows(k) / 2, 
                                             x + j - columns(k) / 2);
            }
        }
        
        return sum;
    }
    
    /**
     * Represents a matrix resulting from the convolution of a matrix
     * with an image kernel.
     * 
     * @tparam K The kernel matrix's type
     * @tparam M The image matrix's type
     */
    template <typename K, typename M>
    struct convolution {
    private:
        K k;
        M m;
    public:
        /**
         * Constructs a new convolved matrix.
         * 
         * @param k The kernel matrix
         * @param m The image matrix
         */
        template <typename T, typename U>
        convolution(T &&k, U &&m) : k(std::forward<T>(k)), m(std::forward<U>(m)) {}
        
        /**
         * Returns the value of the cell at the given coordinates in this matrix.
         * 
         * @param y The cell's row index
         * @param x The cell's column index
         * @return The value of the cell at the position (y,x)
         */
        template <typename T>
        auto get(T y, T x) const -> decltype(conv(k, m, y, x)) {
            return conv(k, m, y, x);
        }
        
        /**
         * Returns the number of rows within this matrix.
         */
        auto rows() const -> decltype(rows(m))  {
            using raster::rows;
            return rows(m);
        }
        
        /**
         * Returns the number of colums within this matrix.
         */
        auto columns() const -> decltype(columns(m)) {
            using raster::columns;
            return columns(m);
        }
    };
    
    /**
     * Constructs a convolved matrix from the given kernel and image matrices.
     * 
     * @param k The kernel matrix
     * @param m The image matrix
     * @return A matrix representing the result of convolving m with k
     */
    template <typename K, typename M>
    convolution<K,M> convolved(K &&k, M &&m) {
        return convolution<K,M>(std::forward<K>(k), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_CONV_HH */
