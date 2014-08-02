#ifndef RASTER_FLT_GAUSSIAN_HH
#define RASTER_FLT_GAUSSIAN_HH

/**
 * Gaussian filter and filter kernel implementation.
 */

#include "../types.hh"

namespace raster {
    namespace fn {
        /**
         * Computes the two-dimensinal gaussian product G(y,x) with the standard deviation sigma.
         *
         * @param y The distance from the origin in the horizontal axis.
         * @param x The distance from the origin in the vertical axis.
         * @param sigma The standard deviation of the distribution function.
         * @return The two-dimensional gaussian product G(y,x,sigma)
         */ 
        double gaussian(double y, double x, double sigma);
    }
    namespace krn {
        /**
         * Convolves the given matrix with a gaussian function (Gaussian Blur).
         * 
         * @param k The matrix to be blurred
         * @param sigma The gaussian function's standard deviation
         * @return The (unnormalized) sum of matrix's values, before the gaussian filter was applied
         */
        template <typename K>
        double gaussian(K &&k, double sigma) {
            double sum = 0;
            
            for (decltype(rows(k)) i = 0; i < rows(k); ++i) {
                for (decltype(columns(k)) j = 0; j < columns(k); ++j) {
                    double gij = fn::gaussian(static_cast<int>(i) - static_cast<int>(rows(k)) / 2, 
                                              static_cast<int>(j) - static_cast<int>(columns(k)) / 2, 
                                              sigma);
                    set(k, i, j, gij);
                    sum += gij;
                }
            }
            
            for (decltype(rows(k)) i = 0; i < rows(k); ++i) {
                for (decltype(columns(k)) j = 0; j < columns(k); ++j) {
                    set(k, i, j, get(k, i, j) / sum);
                }
            }
            
            return sum;
        }
    }
}

#endif /* #ifndef RASTER_FLT_GAUSSIAN_HH */
