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
#define _USE_MATH_DEFINES
#include <cmath>

namespace img {
    template <typename ForwardIterator, typename T, typename P>
    void ellipfit (ForwardIterator begin, ForwardIterator end,
                   T majormin, T minormax, P p, 
                   unsigned int threshold = 4)
    {
        using std::get;
        std::vector<unsigned int> acc(minormax);
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
                    {
                        double b = std::distance(acc.begin(), 
                                                 std::max_element(acc.begin(), 
                                                                  acc.end()));
                        if (acc[b] > threshold) {
                            float orientation = atan2(p1y - p2y, p1x - p2x);
                            if (orientation != 0) {
                                orientation = M_PI - orientation;
                                if (orientation > M_PI) {
                                    orientation = orientation - M_PI / 2;
                                    std::swap(a, b);
                                }
                            }
                            p(cx, cy, a, b, orientation);
                        }
                        std::fill(acc.begin(), acc.end(), 0);
                    }
                }
            }
        }
    }
}

#endif /* #ifndef IMG_ELLIPFIT_H */
