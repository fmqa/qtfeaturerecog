#ifndef CONV_H
#define CONV_H

#include <cstddef>

namespace rst {
    template <typename K, typename T, typename F, typename U>
    auto conv (K k, 
               T m, T n, 
               F f, U y, U x) -> decltype(k(m,n) * f(x,y)) 
    {
        decltype(k(m,n) * f(x,y)) sum{};
        for (T i = 0; i < m; ++i) {
            for (T j = 0; j < n; ++j) {
                sum += k(i, j) * f(y + i - m / 2, x + j - n / 2);
            }
        }
        return sum;
    }
    
    template <typename K, typename T, typename F, typename U>
    auto conv (const K *k, 
               T m, T n, 
               F f, U y, U x) -> decltype((*k) * f(x,y)) 
    {
        return conv([k,m,n](T y, T x) { return k[y * n + x]; },
                    m, n, f, y, x);
    }
    
    template <typename K, int M, int N, typename F, typename T>
    auto conv (const K (&k)[M][N], 
               F f, T y, T x) -> decltype(k[M][N] * f(x,y)) 
    {
        return conv(&k[0][0], M, N, f, y, x);
    }
}

#endif /* #ifndef CONV_H */
