#ifndef IMG_SOBEL_H 
#define IMG_SOBEL_H

#include <utility>

namespace img {
    template <typename T, typename F>
    auto sobel (T x, T y, F f) -> std::pair<decltype(f(x,y)),decltype(f(x,y))> {
        return std::make_pair(
            -1 * f(x-1, y-1) +  1 * f(x+1, y-1) +
            -2 * f(x-1, y)   +  2 * f(x+1, y) +
            -1 * f(x-1, y+1) +  1 * f(x+1, y+1),
             1 * f(x-1, y-1) +  2 * f(x, y-1) +  1 * f(x+1, y-1) +
            -1 * f(x-1, y+1) + -2 * f(x, y+1) + -1 * f(x+1, y+1)
        );
    }
    
    template <typename T, typename F, typename G, typename H>
    void sobel (T width, T height, F f, G g, H h) {
        for (T i = 0; i < height; ++i) {
            for (T j = 0; j < width; ++j) {
                auto gxy = sobel(j, i, f);
                g(j, i) = h(gxy.first, gxy.second);
            }
        }
    }
}

#endif /* #ifndef IMG_SOBEL_H */
