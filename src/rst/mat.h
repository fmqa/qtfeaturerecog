#ifndef RST_MAT_H
#define RST_MAT_H

namespace rst {
    template <typename F, typename T>
    struct mat {
    private:
        F fn;
        T height;
        T width;
    public:
        mat (F f, T m, T n) : fn(f), height(m), width(n) {}
        
        template <typename U>
        auto operator() (U y, U x) -> decltype(fn(y, x)) {
            return fn(y, x);
        }
        
        T rows() const { return height; }
        T columns() const { return width; }
    };
    
    /// Turns a 2D-Function into a matrix by decorating it with
    /// dimensional information.
    /*!
     * \param fn A 2D-Function.
     * \param m The row count of the matrix.
     * \param n The column count of the matrix.
     */
    template <typename F, typename T>
    mat<F,T> asmat (F fn, T m, T n) {
        return mat<F,T>(fn, m, n);
    }
}

#endif /* #ifndef RST_MAT_H */
