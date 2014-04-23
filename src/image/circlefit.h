#ifndef IMG_CIRCLEFIT_H
#define IMG_CIRCLEFIT_H

#include <utility>
#define _USE_MATH_DEFINES
#include <cmath>

namespace img {
    template <typename InputIterator, typename T, typename F, typename P>
    void circlefit (InputIterator begin, InputIterator end, 
                    T width, T height, 
                    F f, P p) 
    {
        for (; begin != end; ++begin) {
            for (T i = 0; i < height; ++i) {
                for (T j = 0; j < width; ++j) {
                    if (f(j, i)) {
                        for (int theta = 0; theta < 360; ++theta) {
                            T x = j + (*begin) * cos(theta * M_PI / 180);
                            T y = i + (*begin) * sin(theta * M_PI / 180);
                            if (x >= 0 && x < width && y >= 0 && y < height) {
                                ++p(x, y, *begin);
                            }
                        }
                    }
                }
            }
        }
    }
    
    template <typename ForwardIterator, typename InputIterator, typename P>
    void circlefit (InputIterator edgebegin, InputIterator edgeend,
                    ForwardIterator radiusbegin, ForwardIterator radiusend,
                    P p)
    {
        using std::get;
        for (; edgebegin != edgeend; ++edgebegin) {
            for (ForwardIterator r = radiusbegin; r != radiusend; ++r) {
                for (int theta = 0; theta < 360; ++theta) {
                    p(get<0>(*edgebegin) + (*r) * cos(theta * M_PI / 180),
                      get<1>(*edgebegin) + (*r) * sin(theta * M_PI / 180),
                      *r);
                }
            }
        }
    }
}

#endif /* #ifndef IMG_CIRCLEFIT_H */
