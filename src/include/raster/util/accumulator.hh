#ifndef RASTER_ACCUMULATOR_HH
#define RASTER_ACCUMULATOR_HH

#include "../types.hh"

namespace raster {
    template <typename M, typename X, typename Y>
    class accumulative_iterator : public std::iterator<std::output_iterator_tag,void,void,void,void> {
        M matrix;
        X x;
        Y y;
    public:
        template <typename T, typename U, typename V>
        accumulative_iterator(T &&m, U &&x, V &&y) 
        : matrix(std::forward<T>(m)),
          x(std::forward<U>(x)),
          y(std::forward<U>(y))
          {}
        
        template <typename T>
        accumulative_iterator& operator=(const T &p) {
            set(matrix, y(p), x(p), get(matrix, y(p), x(p)) + 1);
            return *this;
        }
        
        accumulative_iterator& operator*() { return *this; }
        accumulative_iterator& operator++() { return *this; }
        accumulative_iterator& operator++(int) { return *this; }
    };
    
    template <typename M, typename X, typename Y>
    accumulative_iterator<M,X,Y> accumulator(M &&m, X &&x, Y &&y) {
        return accumulative_iterator<M,X,Y>(std::forward<M>(m),
                                            std::forward<X>(x),
                                            std::forward<Y>(y));
    }
}

#endif /* #ifndef RASTER_ACCUMULATOR_HH */
