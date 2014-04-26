#ifndef CANNY_H
#define CANNY_H

#include "nms.h"
#include "hyst.h"

namespace rst {
    template <typename F, typename G, typename H, typename T, typename U>
    void canny (F f, G g, H h, T m, T n, U tmin, U tmax) {
        rst::hyst([f,h](T y, T x){return rst::nms(f, h, y, x);}, 
                  g, m, n, tmin, tmax);
    }
}

#endif /* #ifndef CANNY_H */
