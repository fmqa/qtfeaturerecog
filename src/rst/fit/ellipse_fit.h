#ifndef RST_ELLIPSE_FIT_H 
#define RST_ELLIPSE_FIT_H

#include <utility>
#define _USE_MATH_DEFINES
#include <cmath>

namespace rst {
    template <typename T>
    struct ellipse_candidate {
        explicit ellipse_candidate (T x = T(), T y = T(),
                                    double a = 0.0, double b = 0.0,
                                    double ort = 0.0) : x(x), y(y),
                                                        a(a), b(b),
                                                        orientation(ort) {}
                                                            
        T x, y;
        double a, b, orientation;
    };
    
    template <typename ForwardIterator, typename T, typename P>
    void ellipse_fit (ForwardIterator begin, ForwardIterator end,
                      T majormin, T minormax, P p)
    {
        using std::get;
        T maxbsqr = minormax * minormax;
        for (ForwardIterator i = begin; i != end; ++i) {
            auto p1x = get<0>(*i);
            auto p1y = get<1>(*i);
            for (ForwardIterator j = begin; j != i; ++j) {
                auto p2x = get<0>(*j);
                auto p2y = get<1>(*j);
                auto dp1p2x = p1x - p2x;
                auto dp1p2y = p1y - p2y;
                double a = 0.5 * sqrt(dp1p2x * dp1p2x + dp1p2y * dp1p2y);
                if (a > 0.5 * majormin) {
                    double cx = 0.5 * (p1x + p2x);
                    double cy = 0.5 * (p1y + p2y);
                    for (ForwardIterator k = begin; k != end; ++k) {
                        auto p3x = get<0>(*k);
                        auto p3y = get<1>(*k);
                        auto dp3cx = p3x - cx;
                        auto dp3cy = p3y - cy;
                        double d = sqrt(dp3cx * dp3cx + dp3cy * dp3cy);
                        if (d > majormin) {
                            auto dp3p1x = p3x - p1x;
                            auto dp3p1y = p3y - p1y;
                            double asqr = a * a;
                            double dsqr = d * d;
                            double fsqr = dp3p1x * dp3p1x + dp3p1y * dp3p1y;
                            double costau = (asqr + dsqr - fsqr) / (2 * a * d);
                            double costausqr = costau * costau;
                            double l = asqr - dsqr * costausqr;
                            if (l > 0 && costausqr < 1) {
                                double bsqr = asqr * 
                                              dsqr * (1 - costausqr) / l;
                                if (bsqr <= maxbsqr) {
                                    double b = sqrt(bsqr);
                                    double orientation = atan2(p1y - p2y, 
                                                               p1x - p2x);
                                    if (orientation != 0) {
                                        orientation = M_PI - orientation;
                                        if (orientation > M_PI) {
                                            orientation -= M_PI / 2;
                                            std::swap(a, b);
                                        }
                                    }
                                    p.candidate(
                                        ellipse_candidate<T>(cx, 
                                                             cy, 
                                                             a, 
                                                             b, 
                                                             orientation));
                                }
                            }
                        }
                    }
                    p.reset();
                }
            }
        }
    }
}

#endif /* #ifndef RST_ELLIPSE_FIT_H */
