#ifndef UTIL_ELLIPSEHOUGH_HH
#define UTIL_ELLIPSEHOUGH_HH

#include <array>
#define _USE_MATH_DEFINES
#include <cmath>
#include <util/ellipses.hh>

namespace mm {
    struct ellipserange {
        int minminor, maxminor;
        int minmajor, maxmajor;
        int minscore, maxscore;
    };
    
    template <typename FirstForwardIterator, typename SecondForwardIterator, typename E, typename G, typename P>
    void ellipsehough(const ellipserange &r,
                      FirstForwardIterator firstangle, FirstForwardIterator lastangle,
                      SecondForwardIterator firstpoint, SecondForwardIterator lastpoint, 
                      E &&edge, G &&candidate, P &&progress)
    {
        for (auto first = firstpoint; first != lastpoint; ++first) {
            for (auto second = firstpoint; second != first; ++second) {
                for (auto third = firstpoint; third != lastpoint; ++third) {
                    double cx = 0, cy = 0, smaj = 0, smin = 0, alpha = 0;
                    if (ellipses::parametrize(first->first, first->second, 
                                              second->first, second->second,
                                              third->first, third->second,
                                              edge, cx, cy, smaj, smin, alpha))
                    {
                        if (smin >= r.minminor / 2 && smin <= r.maxminor / 2 &&
                            smaj >= r.minmajor / 2 && smaj <= r.maxmajor / 2)
                        {
                            int score = ellipses::score(cx, cy, smaj, smin, alpha != 0 ? M_PI - alpha : alpha, 
                                                        ellipses::trig(firstangle, lastangle),
                                                        edge);
                            if (score >= r.minscore && score <= r.maxscore) {
                                candidate(cx, cy, smin, smaj, alpha + M_PI / 2);
                            }
                        }
                    }
                    progress(first->first, first->second,
                             second->first, second->second,
                             third->first, third->second);
                }
            }
        }
    }
}

#endif /* #ifndef UTIL_ELLIPSEHOUGH_HH */
