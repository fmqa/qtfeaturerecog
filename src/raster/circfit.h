#ifndef CIRCFIT_H
#define CIRCFIT_H

#include <utility>
#include "circpath.h"

namespace rast {
    template <typename ForwardIterator, 
              typename InputIterator, 
              typename CandidateFn, 
              typename CircPathMaker>
    void circfit (InputIterator edgebegin, InputIterator edgeend,
                  ForwardIterator radiusbegin, ForwardIterator radiusend,
                  CandidateFn cdt,
                  CircPathMaker p = makestdcircpath)
    {
        using std::get;
        for (; edgebegin != edgeend; ++edgebegin) {
            for (ForwardIterator r = radiusbegin; r != radiusend; ++r) {
                for (const auto &cxy : p(*r, get<0>(*edgebegin), get<1>(*edgebegin))) {
                    cdt(get<0>(cxy), get<1>(cxy), *r);
                }
            }
        }
    }
}

#endif /* #ifndef CIRCFIT_H */
