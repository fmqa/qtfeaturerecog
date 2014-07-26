#ifndef RASTER_FILTER_NMS_HH
#define RASTER_FILTER_NMS_HH

#define _USE_MATH_DEFINES
#include <cmath>
#include "sobel.hh"

namespace raster {
    namespace fn {
        template <typename F, typename H, typename T>
        magnitude<H,F> nms(F &&f, H &&h, T y, T x) {
            auto gyx = sobel(f, y, x);
            float d = 8 * fmod(atan2(gyx.first, gyx.second) + M_PI, M_PI) / M_PI;
            auto fyx = h(gyx.first, gyx.second);
            return (((d <= 1 || d > 7) 
                    && fyx > (gyx = sobel(f, y, x - 1), h(gyx.first, gyx.second))
                    && fyx > (gyx = sobel(f, y, x + 1), h(gyx.first, gyx.second)))
                || ((d > 1 && d <= 3) 
                    && fyx > (gyx = sobel(f, y - 1, x + 1), h(gyx.first, gyx.second))
                    && fyx > (gyx = sobel(f, y + 1, x - 1), h(gyx.first, gyx.second)))
                || ((d > 3 && d <= 5) 
                    && fyx > (gyx = sobel(f, y - 1, x), h(gyx.first, gyx.second))
                    && fyx > (gyx = sobel(f, y + 1, x), h(gyx.first, gyx.second)))
                || ((d > 5 && d <= 7) 
                    && fyx > (gyx = sobel(f, y - 1, x - 1), h(gyx.first, gyx.second))
                    && fyx > (gyx = sobel(f, y + 1, x + 1), h(gyx.first, gyx.second))))
                ? fyx
                : 0;  
        }
    }
    
    template <typename F, typename H>
    class nms {
        F fn;
        H mag;
    public:
        template <typename T, typename U>
        nms(T &&f, U &&m) : fn(std::forward<T>(f)), mag(std::forward<U>(m)) {}
        
        template <typename T>
        magnitude<H,F> get(T y, T x) const {
            return fn::nms(fn, mag, y, x);
        }
        
        auto rows() const -> decltype(rows(fn)) {
            using raster::rows;
            return rows(fn);
        }
        
        auto columns() const -> decltype(columns(fn)) {
            using raster::columns;
            return columns(fn);
        }
    };
    
    template <typename F, typename H>
    nms<F,H> nmsed(F &&f, H &&h) {
        return nms<F,H>(std::forward<F>(f), std::forward<H>(h));
    }
}

#endif /* #ifndef RASTER_FILTER_NMS_HH */
