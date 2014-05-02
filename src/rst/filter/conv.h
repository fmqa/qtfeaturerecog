#ifndef RST_FILTER_CONV_H
#define RST_FILTER_CONV_H

#include "../dim.h"

namespace rst {
    /// Returns the result of the application of the 
    /// convolution function k :: (m x n) to the 2D-Function f at the given
    /// point.
    /*!
     * \param k A convolution function.
     * \param m The row count of the convolution matrix.
     * \param n The column count of the convolution matrix.
     * \param f A 2D-Function.
     * \param y The convolution center's y-coordinate.
     * \param x The convolution center's x-coordinate.
     */
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
    
    /// Matrix overload of conv.
    template <typename K, typename F, typename T>
    auto conv (K k, F f, T y, T x) -> decltype(conv(k, rows(k), columns(k),
                                                    f, y, x))
    {
        return conv(k, rows(k), columns(k), f, y, x);
    }
    
    /// Function pointer overload of conv.
    template <typename K, typename T, typename F, typename U>
    auto conv (const K *k, 
               T m, T n, 
               F f, U y, U x) -> decltype((*k) * f(x,y)) 
    {
        return conv([k,m,n](T y, T x) { return k[y * n + x]; },
                    m, n, f, y, x);
    }
    
    /// 2D-Array overload of conv.
    template <typename K, int M, int N, typename F, typename T>
    auto conv (const K (&k)[M][N], 
               F f, T y, T x) -> decltype(k[M][N] * f(x,y)) 
    {
        return conv(&k[0][0], M, N, f, y, x);
    }
}

#endif /* #ifndef RST_FILTER_CONV_H */
