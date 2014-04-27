#ifndef CANNY_H
#define CANNY_H

#include <functional>
#include "nms.h"
#include "hyst.h"

namespace rst {
    template <typename F, typename G, typename H, typename T, typename U>
    void canny (F f, G g, H h, T m, T n, U tmin, U tmax) {
        using namespace std::placeholders;
        rst::hyst(std::bind(nms<F,H,T>, f, h, _1, _2),
                  g, m, n, tmin, tmax);
    }
}

#endif /* #ifndef CANNY_H */
