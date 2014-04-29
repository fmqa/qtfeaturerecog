#ifndef EDGEDEC_H
#define EDGEDEC_H

#include <utility>
#include "rst/wrap.h"
#include "rst/util.h"
#include "rst/filter/gaussian.h"
#include "rst/filter/canny.h"

namespace fr {
    template <typename F, typename G>
    class edgedec {
        rst::gaussian_filter<3> gaussian;
    public:
        rst::ext_wrapper<F,int> in;
        G out;
        double minthresh;
        double maxthresh;
        bool blurring;
        rst::hypot_fn_t<double> hypot;
        
        edgedec(
            F f, G g, int m, int n,
            double mint, double maxt, bool blur = true,
            rst::hypot_fn_t<double> hyp = rst::fasthypot)
        : gaussian(1.0), in(f, m, n), out(g),
          minthresh(mint), maxthresh(maxt), 
          blurring(blur), hypot(hyp) {}
        
        void apply () const {
            if (blurring) {
                rst::canny([this](int y, int x) { 
                               return gaussian.apply(in, y, x); 
                           },
                           out,
                           hypot,
                           in.height, in.width, minthresh, maxthresh);
            } else {
                rst::canny(in, out, hypot,
                           in.height, in.width, minthresh, maxthresh);
            }
        }
    };
    
    template <typename F, typename G>
    edgedec<F,G> make_edgedec(
        F f, G g, int m, int n,
        double mint, double maxt,
        bool blur = true,
        rst::hypot_fn_t<double> hyp = rst::fasthypot)
    {
        return edgedec<F,G>(f, g, m, n, mint, maxt, blur, hyp);
    }
}

#endif /* #ifndef EDGEDEC_H */
