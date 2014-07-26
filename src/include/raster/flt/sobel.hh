#ifndef RASTER_FLT_SOBEL_HH
#define RASTER_FLT_SOBEL_HH

#include <utility>
#include <array>
#include "../conv.hh"

namespace raster {
    namespace krn {
        namespace sobel {
            extern const std::array<std::array<int,3>,3> x;
            extern const std::array<std::array<int,3>,3> y;
        }
    }
    
    template <typename F>
    using gradient = std::pair<result<std::array<std::array<int,3>,3>,F>,
                               result<std::array<std::array<int,3>,3>,F>>;
    
    template <typename H, typename F>
    using magnitude = decltype(
        std::declval<H>()(
            std::declval<typename gradient<F>::first_type>(),
            std::declval<typename gradient<F>::second_type>()
        )
    );
    
    namespace fn {
        template <typename F, typename T>
        gradient<F> sobel(F &&f, T y, T x) {
            return std::make_pair(conv(&krn::sobel::y, f, y, x),
                                  conv(&krn::sobel::x, f, y, x));
        }
        
        template <typename F, typename T, typename U, typename V>
        gradient<F> sobel(F &&f, T y, T x, U &gx, V &gy) {
            return std::make_pair(gy = conv(&krn::sobel::y, f, y, x),
                                  gx = conv(&krn::sobel::x, f, y, x));
        }
    }
}

#endif /* #ifndef RASTER_FLT_SOBEL_HH */
