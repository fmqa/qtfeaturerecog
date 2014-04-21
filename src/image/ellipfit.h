#ifndef IMG_ELLIPFIT_H 
#define IMG_ELLIPFIT_H

/**
 * An implementation of Yonghong Xie & Qiang Ji's modified Hough transform for
 * ellipse detection.
 * 
 * References:
 * 
 * http://coding-experiments.blogspot.de/2011/05/ellipse-detection-in-image-by-using.html
 * http://github.com/scikit-image/scikit-image/blob/master/skimage/transform/_hough_transform.pyx
 * 
 */

#include <algorithm>
#include <limits>
#include <iterator>
#include <vector>
#include <utility>
#include <cmath>

namespace img {
    template <typename T, typename U, typename F, typename P>
    void ellipfit (T majormin, T minormax, U n, F f, P p, 
                   unsigned int threshold = 4)
    {
        using std::get;
        std::vector<unsigned int> acc(minormax);
        T maxbsqr = minormax * minormax;
        for (T i = 0; i < n; ++i) {
            auto p1 = f(i);
            auto p1x = get<0>(p1);
            auto p1y = get<1>(p1);
            for (T j = 0; j < i; ++j) {
                auto p2 = f(j);
                auto p2x = get<0>(p2);
                auto p2y = get<1>(p2);
                auto dp1p2x = p1x - p2x;
                auto dp1p2y = p1y - p2y;
                double a = 0.5 * sqrt(dp1p2x * dp1p2x + dp1p2y * dp1p2y);
                if (a > 0.5 * majormin) {
                    double cx = 0.5 * (p1x + p2x);
                    double cy = 0.5 * (p1y + p2y);
                    for (T k = 0; k < n; ++k) {
                        auto p3 = f(k);
                        auto p3x = get<0>(p3);
                        auto p3y = get<1>(p3);
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
                                double bsqr = asqr * dsqr * (1 - costausqr) / l;
                                if (bsqr <= maxbsqr) {
                                    auto b = static_cast<unsigned int>(sqrt(bsqr));
                                    if (b && acc[b] < std::numeric_limits<unsigned int>::max()) {
                                        ++acc[b];
                                    }
                                }
                            }
                        }
                    }
                    auto b = std::distance(acc.begin(), 
                                           std::max_element(acc.begin(), 
                                                            acc.end()));
                    if (acc[b] > threshold) {
                        p(cx, cy, a, b);
                    }
                    std::fill(acc.begin(), acc.end(), 0);
                }
            }
        }
    }
}

#endif /* #ifndef IMG_ELLIPFIT_H */
