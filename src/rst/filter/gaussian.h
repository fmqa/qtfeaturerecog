#ifndef GAUSSIAN_H
#define GAUSSIAN_H

namespace rst {
    double gaussian (double, double, double);
    
    template <typename T, typename F>
    double gaussian_kernel (T m, T n, double sigma, F k) {
        double sum = 0;
        T row_mean = m / 2;
        T col_mean = n / 2;
        for (T i = 0; i < m; ++i) {
            for (T j = 0; j < n; ++j) {
                sum += k(i, j) = gaussian(row_mean - i, j - col_mean, sigma);
            }
        }
        for (T i = 0; i < m; ++i) {
            for (T j = 0; j < n; ++j) {
                k(i, j) /= sum;
            }
        }
        return sum;
    }
    
    template <typename K, int M, int N>
    double gaussian_kernel (double sigma, K (&k)[M][N]) {
        return gaussian_kernel(M, N, sigma,
                               [&k](int y, int x) -> K& {return k[y][x];});
    }
    
    template <typename T, typename F>
    double gaussian_kernel (T size, double sigma, F k) {
        return gaussian_kernel(size, size, sigma, k);
    }
}

#endif /* #ifndef GAUSSIAN_H */
