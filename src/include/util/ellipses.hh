#ifndef ELLIPSES_HH 
#define ELLIPSES_HH

#include <utility>
#include <iterator>
#include <cmath>
#include "lines.hh"

namespace ellipses {
    template <typename T,
              typename InputIterator,
              typename OutputIterator,
              typename U = int,
              typename V = double>
    OutputIterator trigellipse(T a, 
                               T b, 
                               InputIterator firstangle, 
                               InputIterator lastangle,
                               OutputIterator out,
                               U x0 = U(),
                               U y0 = U(),
                               V alpha = V())
    {
        for (; firstangle != lastangle; ++firstangle) {
            auto x = a * std::cos(*firstangle);
            auto y = b * std::sin(*firstangle);
            *out++ = std::make_pair(x0 + x * std::cos(alpha) - y * std::sin(alpha),
                                    y0 + y * std::cos(alpha) + x * std::sin(alpha));
        }
        return out;
    }
    
    template <typename ForwardIterator>
    class trigonometric {
        ForwardIterator begin, end;
    public:
        trigonometric(ForwardIterator first, ForwardIterator last) : begin(first),
                                                                     end(last) {}
        template <typename T, 
                  typename OutputIterator,
                  typename U = int,
                  typename V = double>
        OutputIterator operator()(T a,
                                  T b,
                                  OutputIterator out,
                                  U x0 = U(),
                                  U y0 = U(),
                                  V alpha = V())
        {
            return trigellipse(a, b, begin, end, out, x0, y0, alpha);
        }
    };
    
    template <typename ForwardIterator>
    trigonometric<ForwardIterator> trig(ForwardIterator firstangle, ForwardIterator lastangle) {
        return trigonometric<ForwardIterator>(firstangle, lastangle);
    }
    
    template <typename T, typename U, typename V, typename F, typename W, typename X, typename Y>
    bool parametrize(T x0, T y0,
                     U x1, U y1,
                     V x2, V y2,
                     F &&f,
                     W &xc, W &yc,
                     X &a, X &b, 
                     Y &alpha,
                     int off = 2)
    {
        if (!f(x0, y0) || !f(x1, y1) || !f(x2, y2)) {
            return false;
        }
        
        double xt0, yt0;
        
        if (!lines::tangent_intersection(x0, y0, x1, y1, xt0, yt0, f, off)) {
            std::swap(x0, x2);
            std::swap(y0, y2);
            if (!lines::tangent_intersection(x0, y0, x1, y1, xt0, yt0, f, off)) {
                return false;
            }
        }
        
        double xt1, yt1;
        
        if (!lines::tangent_intersection(x1, y1, x2, y2, xt1, yt1, f, off)) {
            std::swap(x0, x2);
            std::swap(y0, y2);
            if (!lines::tangent_intersection(x1, y1, x2, y2, xt1, yt1, f, off)) {
                return false;
            }
        }
        
        double h01x = (x0 + x1) / 2.0;
        double h01y = (y0 + y1) / 2.0;
        double h12x = (x1 + x2) / 2.0;
        double h12y = (y1 + y2) / 2.0;
        
        
        if (!lines::intersect(xt0, yt0, h01x, h01y, xt1, yt1, h12x, h12y, xc, yc)) {
            return false;
        }
        
        double dx0 = x0 - xc, dy0 = y0 - yc, 
               dx1 = x1 - xc, dy1 = y1 - yc, 
               dx2 = x2 - xc, dy2 = y2 - yc;
        
        double k = 2 * (dy0 * dx1 - dx0 * dy1) * (dx0 * dy2 * (dy1 * dx2 - dx1 * dy2) + 
                                                  dy0 * (dx1 * dx2 * dy2 - dy1 * dy1 * dy1));
        
        if (k == 0) {
            return false;
        }
        
        double A = (2 * (dx0 * dy0 * (dy1 * dy1 - dy2 * dy2) +
                         dy0 * dy0 * (dx2 * dy2 - dx1 * dy1) +
                         dy1 * dy2 * (dx1 * dy2 - dy1 * dx2))) / k;
        
        double B = (dy1 * dy1 * (dy1 * dy1 - dx0 * dx0 - dy0 * dy0) + 
                    dy2 * dy2 * (dx0 * dx0 - dx1 * dx1) + 
                    dy0 * dy0 * dx1 * dx1) / k;
        
        double C = (2 * (dx0 * dx0 * (dx1 * dy1 - dx2 * dy2) +
                         dx0 * dy0 * (dy1 * dy1 - dx1 * dx1) +
                         dx1 * (dx1 * dx2 * dy2 - dy1 * dy1 * dy1))) / k;
        
                    
        double D = 4 * A * C - B * B;
        
        if (D <= 0) {
            return false;
        }
        
        a = std::sqrt(1 / std::min(std::abs(A), std::abs(C)));
        b = std::sqrt(1 / std::max(std::abs(A), std::abs(C)));
        alpha = std::atan((C - A + std::sqrt((C - A) * (C - A) + B * B)) / B);

        return true;
    }
    
    template <typename T, typename U, typename V, typename Ellipse, typename F>
    int score(T x0, T y0, U smaj, U smin, V alpha, Ellipse &&e, F &&f, int off = 1) {
        class iterator {
            Ellipse e;
            F f;
            int *score;
            int off;
        public:
            iterator(Ellipse e, F f, int *s, int o) : e(e), f(f), score(s), off(o) {}
            iterator& operator=(const std::pair<T,T> &p) {
                for (int y = -off; y <= off; ++y) {
                    for (int x = -off; x <= off; ++x) {
                        if (f(p.first + x, p.second + y)) {
                            ++*score;
                            return *this;
                        }
                    }
                }
                return *this;
            }
            iterator& operator*() { return *this; }
            iterator& operator++() { return *this; }
            iterator& operator++(int) { return *this; }
        };
        int score = 0;
        return e(smaj, smin, iterator(e, f, &score, off), x0, y0, alpha), score;
    }
}

#endif /* #ifndef ELLIPSES_HH */
