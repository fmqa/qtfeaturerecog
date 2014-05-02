#ifndef RST_WRAP_H
#define RST_WRAP_H

#include "compose.h"

namespace rst {
    template <typename F, typename T, typename U>
    auto extendwrap (F f, T m, T n, U y, U x) -> decltype(f(y,x)) {
        if (x >= 0 && x < n) {
            if (y < 0) {
                return f(0, x);
            }
            if (y >= m) {
                return f(m - 1, x);
            }
        }
        if (y >= 0 && y < m) {
            if (x < 0) {
                return f(y, 0);
            }
            if (x >= n) {
                return f(y, n - 1);
            }
        }
        if (y < 0) {
            if (x < 0) {
                return f(0, 0);
            }
            if (x >= n) {
                return f(0, n - 1);
            }
        }
        if (y >= m) {
            if (x < 0) {
                return f(m - 1, 0);
            }
            if (x >= n) {
                return f(m - 1, n - 1);
            }
        }
        return f(y, x);
    }
    
    template <typename F, typename T>
    auto extendwrap (F f, T y, T x) 
        -> decltype(extendwrap(f, rows(f), columns(f), y, x))
    {
        return extendwrap(f, rows(f), columns(f), y, x);
    }
    
    template <typename T, typename M>
    auto bndextwrap (M m) -> decltype(mbind(extendwrap<M,T>, m)) {
        return mbind(extendwrap<M,T>, m);
    }
}

#endif /* #ifndef RST_WRAP_H */
