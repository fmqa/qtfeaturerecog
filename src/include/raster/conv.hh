#ifndef RASTER_CONV_HH
#define RASTER_CONV_HH

#include "traits.hh"

namespace raster {
    template <typename T, typename U>
    using result = decltype(
        std::declval<typename traits<T>::value_type>() *
        std::declval<typename traits<U>::value_type>()
    );
    
    template <typename K, typename F, typename T>
    auto conv(K &&k, F &&f, T y, T x) -> result<K,F> {
        result<K,F> sum{};
        
        for (decltype(rows(k)) i = 0; i < rows(k); ++i) {
            for (decltype(columns(k)) j = 0; j < columns(k); ++j) {
                sum += get(k, i, j) * get(f, y + i - rows(k) / 2, 
                                             x + j - columns(k) / 2);
            }
        }
        
        return sum;
    }
    
    template <typename K, typename M>
    struct convolution {
    private:
        K k;
        M m;
    public:
        template <typename T, typename U>
        convolution(T &&k, U &&m) : k(std::forward<T>(k)), m(std::forward<U>(m)) {}
        
        template <typename T>
        auto get(T y, T x) const -> decltype(conv(k, m, y, x)) {
            return conv(k, m, y, x);
        }
        
        auto rows() const -> decltype(rows(m))  {
            using raster::rows;
            return rows(m);
        }
        
        auto columns() const -> decltype(columns(m)) {
            using raster::columns;
            return columns(m);
        }
    };
    
    template <typename K, typename M>
    convolution<K,M> convolved(K &&k, M &&m) {
        return convolution<K,M>(std::forward<K>(k), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_CONV_HH */
