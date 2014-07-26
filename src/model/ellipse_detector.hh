#ifndef MODEL_ELLIPSEDETECTOR_HH
#define MODEL_ELLIPSEDETECTOR_HH

#include <array>
#define _USE_MATH_DEFINES
#include <cmath>
#include <util/ellipses.hh>

namespace mm {
    class ellipse_detector {
        int min_minor;
        int max_minor;
        int min_major;
        int max_major;
        int min_score;
        int max_score;
        std::array<double,360> ucirc_angles;
    public:
        explicit ellipse_detector(int minmnr = 40, int maxmnr = 60, int minmjr = 70, int maxmjr = 100,
                                  int minscr = 100, int maxscr = 9999)
        : min_minor(minmnr), max_minor(maxmnr),
          min_major(minmjr), max_major(maxmjr),
          min_score(minscr), max_score(maxscr)
        {
            for (size_t i = 0; i < ucirc_angles.size(); ++i) {
                ucirc_angles[i] = i * M_PI / 180;
            }
        }
        
        int minimum_minor() const {
            return min_minor;
        }
        
        ellipse_detector& with_minimum_minor(int value) {
            min_minor = value;
            return *this;
        }
        
        int maximum_minor() const {
            return max_minor;
        }
        
        ellipse_detector& with_maximum_minor(int value) {
            max_minor = value;
            return *this;
        }
        
        int minimum_major() const {
            return min_major;
        }
        
        ellipse_detector& with_minimum_major(int value) {
            min_major = value;
            return *this;
        }
        
        int maximum_major() const {
            return max_major;
        }
        
        ellipse_detector& with_maximum_major(int value) {
            max_major = value;
            return *this;
        }
        
        int minimum_score() const {
            return min_score;
        }
        
        ellipse_detector& with_minimum_score(int value) {
            min_score = value;
            return *this;
        }
        
        int maximum_score() const {
            return max_score;
        }
        
        ellipse_detector& with_maximum_score(int value) {
            max_score = value;
            return *this;
        }
        
        template <typename ForwardIterator, typename E, typename G, typename P>
        void apply(ForwardIterator firstpoint, ForwardIterator lastpoint,
                   E &&edge, G &&candidate, P &&progress) const
        {
            for (ForwardIterator first = firstpoint; first != lastpoint; ++first) {
                for (ForwardIterator second = firstpoint; second != first; ++second) {
                    for (ForwardIterator third = firstpoint; third != lastpoint; ++third) {
                        double cx = 0, cy = 0, smaj = 0, smin = 0, alpha = 0;
                        if (ellipses::parametrize(first->first, first->second, 
                                                  second->first, second->second,
                                                  third->first, third->second,
                                                  edge, cx, cy, smaj, smin, alpha))
                        {
                            if (smin >= min_minor / 2 && smin <= max_minor / 2 &&
                                smaj >= min_major / 2 && smaj <= max_major / 2)
                            {
                                int score = ellipses::score(cx, cy, smaj, smin, alpha != 0 ? M_PI - alpha : alpha, 
                                                            ellipses::trig(ucirc_angles.begin(), ucirc_angles.end()),
                                                            edge);
                                if (score >= min_score && score <= max_score) {
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
    };
}

#endif /* #ifndef MODEL_ELLIPSEDETECTOR_HH */
