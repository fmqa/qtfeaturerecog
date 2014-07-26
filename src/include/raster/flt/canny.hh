#ifndef RASTER_FILTER_CANNY_HH
#define RASTER_FILTER_CANNY_HH

#include "nms.hh"
#include "hyst.hh"

namespace raster {
    template <typename M, typename G, typename H, typename T>
    void canny(M &&m, G &&g, H &&h, T tmin, T tmax) {
        hyst(nmsed(m, h), g, tmin, tmax);
    }
}

#endif /* #ifndef RASTER_FILTER_CANNY_HH */
