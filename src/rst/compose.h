#ifndef RST_COMPOSE_H
#define RST_COMPOSE_H

#include "dim.h"

namespace rst {
    template <typename K, typename F>
    struct bound {
    private:
        K convfn;
        F fn;
    public:
        bound (K k, F f) : convfn(k), fn(f) {}
        
        template <typename T>
        auto operator()(T y, T x) -> decltype(convfn(fn, y, x)) {
            return convfn(fn, y, x);
        }
    };
    
    template <typename K, typename M>
    struct boundm {
    private:
        K convfn;
        M mat;
    public:
        boundm (K k, M m) : convfn(k), mat(m) {}
        
        template <typename T>
        auto operator()(T y, T x) -> decltype(convfn(mat, y, x)) {
            return convfn(mat, y, x);
        }
        
        auto rows() const -> decltype(rows(mat))  {
            using rst::rows;
            return rows(mat);
        }
        
        auto columns() const -> decltype(columns(mat)) {
            using rst::columns;
            return columns(mat);
        }
    };
    
    /// Binds a convolution function k(f,y,x) to 2D-Function f(y,x).
    /// Returns a callable 2D-Function object that returns the result
    /// the result of the convolution k at the given point.
    /*!
     * \param k A convolution function.
     * \param f A 2D-Function.
     */
    template <typename K, typename F>
    bound<K,F> fbind (K k, F f) {
        return bound<K,F>(k, f);
    }
    
    /// Binds a convolution function k(f,y,x) to a matrix-view with defined
    /// dimensions.
    /// Returns a matrix-view object that has the same dimensions as the given
    /// matrix-view, and has the same values of the convolution k applied to m.
    /*!
     * \param k A convolution function.
     * \param m A matrix.
     */
    template <typename K, typename M>
    boundm<K,M> mbind (K k, M m) {
        return boundm<K,M>(k, m);
    }
}

#endif /* #ifndef RST_COMPOSE_H */

