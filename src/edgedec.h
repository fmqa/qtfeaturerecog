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
        rst::ext_wrapper<F,int> in;
        G out;
        rst::gaussian_filter<3> gaussian;
        double minthresh;
        double maxthresh;
        bool blurring;
        double (*hypot)(const std::pair<double,double> &);
    public:
        edgedec (
            F f, G g, int m, int n,
            double minthrsh, double maxthrsh, bool blur = true,
            double (*hyp)(const std::pair<double,double> &) = rst::fasthypot)
        : in(f, m, n), out(g), gaussian(1.0),
          minthresh(minthrsh), maxthresh(maxthrsh), 
          blurring(blur), hypot(hyp) {}
        
        double min_threshold () const {
            return minthresh;
        }
        
        double min_threshold (double mt) {
            double old = minthresh;
            minthresh = mt;
            return old;
        }
        
        double max_threshold () const {
            return maxthresh;
        }
        
        double max_threshold (double mt) {
            double old = maxthresh;
            maxthresh = mt;
            return old;
        }
        
        bool blur () const {
            return blurring;
        }
        
        bool blur (bool blr) {
            bool old = blurring;
            blurring = blr;
            return old;
        }
        
        auto hypotfn () const -> double(*)(const std::pair<double,double> &) {
            return hypot;
        }
        
        auto hypotfn (double (*h)(const std::pair<double,double> &)) 
            -> double(*)(const std::pair<double,double> &) 
        {
            double (*old)(const std::pair<double,double> &) = hypot;
            hypot = h;
            return old;
        }
        
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
        double (*hyp)(const std::pair<double,double> &) = rst::fasthypot)
    {
        return edgedec<F,G>(f, g, m, n, mint, maxt, blur, hyp);
    }
}

#endif /* #ifndef EDGEDEC_H */
