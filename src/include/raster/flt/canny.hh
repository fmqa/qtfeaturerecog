#ifndef RASTER_FILTER_CANNY_HH
#define RASTER_FILTER_CANNY_HH

/**
 * Canny edge detection algorithm implementation.
 */

#include "nms.hh"
#include "hyst.hh"

namespace raster {
    /**
     * Extracts the edges of a given image matrix.
     * 
     * @param m The matrix whose edges extracted
     * @param g Output matrix for the extracted edges
     * @param h Magnitude function
     * @param tmin Minimum threshold for an edge to be considered
     * @param tmax Maximum threshold for an edge to be considered
     */
    template <typename M, typename G, typename H, typename T>
    void canny(M &&m, G &&g, H &&h, T tmin, T tmax) {
        hyst(nmsed(m, h), g, tmin, tmax);
    }
}

#endif /* #ifndef RASTER_FILTER_CANNY_HH */
