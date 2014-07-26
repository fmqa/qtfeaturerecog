#ifndef RASTER_FLT_GAUSSIAN_HH
#define RASTER_FLT_GAUSSIAN_HH

#include "../types.hh"

namespace raster {
    namespace fn {
        double gaussian(double, double, double);
    }
    namespace krn {
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
