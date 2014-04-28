#ifndef RST_CIRCLE_FIT_H
#define RST_CIRCLE_FIT_H

#include <utility>

namespace rst {
    template <typename ForwardIterator, 
              typename InputIterator, 
              typename C, 
              typename F>
    void circle_fit (InputIterator edgebegin, InputIterator edgeend,
                     ForwardIterator radiusbegin, ForwardIterator radiusend,
                     C c, F f)
    {
        using std::get;
        for (; edgebegin != edgeend; ++edgebegin) {
            for (ForwardIterator r = radiusbegin; r != radiusend; ++r) {
                for (const auto &cxy : c(*r, get<0>(*edgebegin), get<1>(*edgebegin))) {
                    f(get<0>(cxy), get<1>(cxy), *r);
                }
            }
        }
    }
}

#endif /* #ifndef RST_CIRCLE_FIT_H */
