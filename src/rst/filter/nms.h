#ifndef NMS_H
#define NMS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "sobel.h"

namespace rst {
    template <typename F, typename H, typename T>
    auto nms (F f, H h, T y, T x) -> decltype(h(sobel(f, y, x))) {
        auto gyx = sobel(f, y, x);
        float d = 8 * fmod(atan2(gyx.first, gyx.second) + M_PI, M_PI) / M_PI;
        auto fyx = h(sobel(f, y, x));
        return (((d <= 1 || d > 7) 
                && fyx > h(sobel(f,y,x-1)) 
                && fyx > h(sobel(f,y,x+1)))
            || ((d > 1 && d <= 3) 
                && fyx > h(sobel(f,y-1,x+1)) 
                && fyx > h(sobel(f,y+1,x-1)))
            || ((d > 3 && d <= 5) 
                && fyx > h(sobel(f,y-1,x)) 
                && fyx > h(sobel(f,y+1,x)))
            || ((d > 5 && d <= 7) 
                && fyx > h(sobel(f,y-1,x-1)) 
                && fyx > h(sobel(f,y+1,x+1))))
            ? fyx
            : 0;  
    }
}

#endif /* #ifndef NMS_H */
