#ifndef RST_UTIL_H
#define RST_UTIL_H

#include <utility>
#include <cmath>

namespace rst {
    namespace __rst_util_detail {
        using std::get;
        
        template <typename T>
        auto trghypot (const T &t) -> 
            decltype(std::hypot(get<0>(t), get<1>(t))) 
        {
            return std::hypot(get<0>(t), get<1>(t));
        }
        
        template <typename T>
        auto fasthypot(const T &t) -> 
            decltype(std::sqrt(get<0>(t) * get<0>(t) +
                               get<1>(t) * get<1>(t)))
        {
            return std::sqrt(get<0>(t) * get<0>(t) + get<1>(t) * get<1>(t));
        }
        
        template <typename T>
        auto sumabs(const T &t) -> 
            decltype(std::abs(get<0>(t)) + std::abs(get<1>(t)))
        {
            return std::abs(get<0>(t)) + std::abs(get<1>(t));
        }
    }
    
    template <typename T, template <typename...> class U = std::pair>
    using hypot_fn_t = T (*)(const U<T,T> &);
    
    template <typename T>
    auto trghypot (const T &t) -> 
        decltype(sizeof(__rst_util_detail::trghypot(t)),
                 __rst_util_detail::trghypot(t))
    {
        return __rst_util_detail::trghypot(t);
    }
    
    template <typename T>
    auto fasthypot (const T &t) -> 
        decltype(sizeof(__rst_util_detail::fasthypot(t)),
                 __rst_util_detail::fasthypot(t))
    {
        return __rst_util_detail::fasthypot(t);
    }
    
    template <typename T>
    auto sumabs (const T &t) -> decltype(sizeof(__rst_util_detail::sumabs(t)),
                                         __rst_util_detail::sumabs(t))
    {
        return __rst_util_detail::sumabs(t);
    }    
}

#endif /* #ifndef RST_UTIL_H */
