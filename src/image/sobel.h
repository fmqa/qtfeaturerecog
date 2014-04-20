#ifndef IMG_SOBEL_H 
#define IMG_SOBEL_H

namespace img {
    template <typename T, typename F, typename G, typename H>
    void sobel (T width, T height, F f, G g, H h) {
        for (T i = 0; i < height; ++i) {
            for (T j = 0; j < width; ++j) {
                g(j, i) = h(-1 * f(j-1, i-1) +  1 * f(j+1, i-1) +
                            -2 * f(j-1, i)   +  2 * f(j+1, i) +
                            -1 * f(j-1, i+1) +  1 * f(j+1, i+1),
                             1 * f(j-1, i-1) +  2 * f(j, i-1) +  1 * f(j+1, i-1) +
                            -1 * f(j-1, i+1) + -2 * f(j, i+1) + -1 * f(j+1, i+1));
            }
        }
    }
}

#endif /* #ifndef IMG_SOBEL_H */
