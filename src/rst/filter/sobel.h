#ifndef RST_FILTER_SOBEL_H 
#define RST_FILTER_SOBEL_H

#include <utility>
#include "conv.h"

namespace rst {
    extern const int sobel_x_kernel[3][3], sobel_y_kernel[3][3];
    
    template <typename F, typename T>
    auto sobel (F f, T y, T x) 
        -> decltype(std::make_pair(conv(sobel_y_kernel, f, y, x),
                                   conv(sobel_x_kernel, f, y, x)))
    {
        return std::make_pair(conv(sobel_y_kernel, f, y, x),
                              conv(sobel_x_kernel, f, y, x));
    }
}

#endif /* #ifndef RST_FILTER_SOBEL_H */
