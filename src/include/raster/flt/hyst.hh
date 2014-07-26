#ifndef RASTER_FILTER_HYST_HH
#define RASTER_FILTER_HYST_HH

#include <limits>
#include "../types.hh"

namespace raster {
    template <typename M, typename G, typename T, typename U>
    void hyst_mark_edge(M &&m, G &&g, T i, T j, U tmin) {
        T y0 = i ? i - 1 : i;
        T x0 = j ? j - 1 : j;
        T y1 = (i == rows(m) - 1) ? i : i + 1;
        T x1 = (j == columns(m) - 1) ? j : j + 1;
        set(g, i, j, std::numeric_limits<decltype(get(g, i, j)+0)>::max());
        for (; y0 <= y1; ++y0) {
            for (; x0 <= x1; ++x0) {
                if ((y0 != y1 || x0 != x1) 
                    && get(g, y0, x0) == 0 
                    && get(m, y0, x0) >= tmin)
                {
                    hyst_mark_edge(m, g, y0, x0, tmin);
                    return;
                }
            }
        }
    }
    
    template <typename M, typename G, typename T>
    void hyst(M &&m, G &&g, T tmin, T tmax) {
        for (decltype(rows(m)) i = 0; i < rows(m); ++i) {
            for (decltype(columns(m)) j = 0; j < columns(m); ++j) {
                if (get(g, i, j) == 0 && get(m, i, j) >= tmax) {
                    hyst_mark_edge(m, g, i, j, tmin);
                }
            }
        }
    }
}

#endif /* #ifndef RASTER_FILTER_HYST_H */

