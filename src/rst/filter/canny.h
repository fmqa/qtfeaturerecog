#ifndef RST_FILTER_CANNY_H
#define RST_FILTER_CANNY_H

#include "nms.h"
#include "hyst.h"

namespace rst {
    template <typename F, typename G, typename H, typename T, typename U>
    void canny (F f, G g, H h, T m, T n, U tmin, U tmax) {
        rst::hyst([f,h](T y, T x){ return nms(f, h, y, x); },
                  g, m, n, tmin, tmax);
    }
    
    template <typename F, typename G, typename H, typename T>
    void canny (F f, G g, H h, T tmin, T tmax) {
        canny (f, g, h, rows(f), columns(f), tmin, tmax);
    }
}

#endif /* #ifndef RST_FILTER_CANNY_H */
