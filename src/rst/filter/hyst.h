#ifndef RST_FILTER_HYST_H
#define RST_FILTER_HYST_H

#include <limits>
#include "../dim.h"

namespace rst {
    template <typename F, typename G, typename T, typename U>
    void mark_edge (F f, G g, T m, T n, T i, T j, U tmin) {
        T y0 = i ? i - 1 : i;
        T x0 = j ? j - 1 : j;
        T y1 = (i == m - 1) ? i : i + 1;
        T x1 = (j == n - 1) ? j : j + 1;
        g(i, j) = std::numeric_limits<decltype(g(i, j)+0)>::max();
        for (; y0 <= y1; ++y0) {
            for (; x0 <= x1; ++x0) {
                if ((y0 != y1 || x0 != x1) 
                    && g(y0, x0) == 0 
                    && f(y0, x0) >= tmin)
                {
                    mark_edge(f, g, m, n, y0, x0, tmin);
                    return;
                }
            }
        }
    }
    
    template <typename F, typename G, typename T, typename U>
    void hyst (F f, G g, T m, T n, U tmin, U tmax) {
        for (T i = 0; i < m; ++i) {
            for (T j = 0; j < n; ++j) {
                if (g(i, j) == 0 && f(i, j) >= tmax) {
                    mark_edge(f, g, m, n, i, j, tmin);
                }
            }
        }
    }
    
    template <typename F, typename G, typename T>
    void hyst (F f, G g, T tmin, T tmax) {
        hyst (f, g, rows(f), columns(f), tmin, tmax);
    }
}

#endif /* #ifndef RST_FILTER_HYST_H */

