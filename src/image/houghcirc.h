#ifndef IMG_HOUGHCIRC_H
#define IMG_HOUGHCIRC_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace img {
    template <typename InputIterator, typename T, typename F, typename P>
    void houghcirc (InputIterator begin, InputIterator end, 
                    T width, T height, 
                    F f, P p) 
    {
        for (; begin != end; ++begin) {
            for (T i = 0; i < height; ++i) {
                for (T j = 0; j < width; ++j) {
                    if (f(j, i)) {
                        for (int theta = 0; theta < 180; ++theta) {
                            T x = j + (*begin) * std::cos(theta * 180 / M_PI);
                            T y = i + (*begin) * std::sin(theta * 180 / M_PI);
                            if (x >= 0 && x < width && y >= 0 && y < height) {
                                ++p(x, y, *begin);
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif /* #ifndef IMG_HOUGHCIRC_H */
