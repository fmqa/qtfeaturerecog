#ifndef RASTER_FLT_SOBEL_HH
#define RASTER_FLT_SOBEL_HH

/**
 * Sobel filter and filter kernel implementation.
 */

#include <utility>
#include <array>
#include "../conv.hh"

namespace raster {
    namespace krn {
        namespace sobel {
            /**
             * The sobel filter kernel in the x-direction.
             */
            extern const std::array<std::array<int,3>,3> x;
            
            /**
             * The sobel filter kernel in the y-direction.
             */
            extern const std::array<std::array<int,3>,3> y;
        }
    }
    
    /**
     * Gradient type of a given matrix type.
     */
    template <typename F>
    using gradient = std::pair<result<std::array<std::array<int,3>,3>,F>,
                               result<std::array<std::array<int,3>,3>,F>>;
    
    /**
     * Magnitude type of a matrix F normalized by the magnitude function H.
     */
    template <typename H, typename F>
    using magnitude = decltype(
        std::declval<H>()(
            std::declval<typename gradient<F>::first_type>(),
            std::declval<typename gradient<F>::second_type>()
        )
    );
    
    namespace fn {
        /**
         * Returns a pair representing the y/x sobel gradients of the matrix
         * cell (y,x)
         * 
         * @param f A matrix
         * @param y The row index of the cell
         * @param x The column index of the cell
         * @return A pair containing the sobel y- and x-gradients, respectively of the cell at (y,x) 
         */
        template <typename F, typename T>
        gradient<F> sobel(F &&f, T y, T x) {
            return std::make_pair(conv(&krn::sobel::y, f, y, x),
                                  conv(&krn::sobel::x, f, y, x));
        }
        
        /**
         * Returns a pair representing the y/x sobel gradients of the matrix
         * cell (y,x)
         * 
         * @param f A matrix
         * @param y The row index of the cell
         * @param x The column index of the cell
         * @param gx Will be filled with the value of the x-gradient
         * @param gy Will be filled with the value of the y-gradient
         * @return A pair containing the sobel y- and x-gradients, respectively of the cell at (y,x) 
         */
        template <typename F, typename T, typename U, typename V>
        gradient<F> sobel(F &&f, T y, T x, U &gx, V &gy) {
            return std::make_pair(gy = conv(&krn::sobel::y, f, y, x),
                                  gx = conv(&krn::sobel::x, f, y, x));
        }
    }
}

#endif /* #ifndef RASTER_FLT_SOBEL_HH */
