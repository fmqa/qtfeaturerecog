#ifndef RST_FILTER_NMS_H
#define RST_FILTER_NMS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "sobel.h"

namespace rst {
    namespace __rst_filter_detail {
        template <typename T, typename U>
        auto proxy(T p, U binaryfn) -> decltype(binaryfn(p.first, p.second)) {
            return binaryfn(p.first, p.second);
        }
    }
    
    template <typename F, typename H, typename T>
    auto nms (F f, H h, T y, T x) 
        -> decltype(__rst_filter_detail::proxy(sobel(f, y, x), h))
    {
        auto gyx = sobel(f, y, x);
        float d = 8 * fmod(atan2(gyx.first, gyx.second) + M_PI, M_PI) / M_PI;
        auto fyx = __rst_filter_detail::proxy(sobel(f, y, x), h);
        return (((d <= 1 || d > 7) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y,x-1), h)
                && fyx > __rst_filter_detail::proxy(sobel(f,y,x+1), h))
            || ((d > 1 && d <= 3) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y-1,x+1), h) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y+1,x-1), h))
            || ((d > 3 && d <= 5) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y-1,x), h) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y+1,x), h))
            || ((d > 5 && d <= 7) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y-1,x-1), h) 
                && fyx > __rst_filter_detail::proxy(sobel(f,y+1,x+1), h)))
            ? fyx
            : 0;  
    }
}

#endif /* #ifndef RST_FILTER_NMS_H */
