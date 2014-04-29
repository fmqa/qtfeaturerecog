#ifndef RST_FILTER_GAUSSIAN_H
#define RST_FILTER_GAUSSIAN_H

#include "conv.h"

namespace rst {
    double gaussian (double, double, double);
    
    template <typename F, typename T>
    double kgaussian (F k, T m, T n, double sigma) {
        double sum = 0;
        
        for (T i = 0; i < m; ++i) {
            for (T j = 0; j < n; ++j) {
                sum += k(i, j) = gaussian(m / 2 - i, j - n / 2, sigma);
            }
        }
        
        for (T i = 0; i < m; ++i) {
            for (T j = 0; j < n; ++j) {
                k(i, j) /= sum;
            }
        }
        
        return sum;
    }
    
    template <typename F, typename T>
    double kgaussian (F k, T size, double sigma) {
        return kgaussian(k, size, size, sigma);
    }
    
    template <typename K, int M, int N>
    double kgaussian (K (&k)[M][N], double sigma) {
        return kgaussian([&k](int y, int x) -> K& { return k[y][x]; },
                         M, N, sigma);
    }
    
    template <int M, int N = M>
    struct gaussian_flt {
        template <typename F>
        struct bound {
            friend struct gaussian_flt;
        private:
            const gaussian_flt *parent;
            F fn;
            bound(const gaussian_flt *p, F f) : parent(p), fn(f) {}
        public:
            template <typename T>
            auto operator()(T y, T x) -> decltype(fn(y, x)) {
                return parent->apply(fn, y, x);
            }
        };
        
        explicit gaussian_flt(double sigma) : kernel() { 
            kgaussian(kernel, sigma); 
        }
        
        template <typename F, typename T>
        double apply (F f, T y, T x) const {
            return conv(kernel, f, y, x);
        }
        
        template <typename F>
        bound<F> bind (F fn) const {
            return bound<F>(this, fn);
        }
    private:
        double kernel[M][N];
    };
}

#endif /* #ifndef RST_FILTER_GAUSSIAN_H */
