#ifndef RASTER_TRANSLATE_HH
#define RASTER_TRANSLATE_HH

/**
 * Algorithms for adjusting access patterns on matrix-like objects.
 */

#include "types.hh"

namespace raster {
    /**
     * Represents a the translation of a matrix by a translation function.
     * 
     * @tparam F The translation functions's type
     * @tparam M The translated matrix's type
     */
    template <typename F, typename M>
    struct translation {
    private:
        F f;
        M m;
    public:
        /**
         * Constructs a translated matrix with f as the translation function.#
         * @param f The translation function
         * @param m The matrix to be translated by f
         */
        template <typename T, typename U>
        translation(T &&f, U &&m) : f(std::forward<T>(f)), m(std::forward<U>(m)) {}

        /**
         * Returns the value of the cell at the given coordinates in this matrix.
         * 
         * @param y The cell's row index
         * @param x The cell's column index
         * @return The value of the cell at the position (y,x)
         */
        template <typename T>
        auto get(T y, T x) const -> decltype(f(m, y, x)) {
            return f(m, y, x);
        }
        
        /**
         * Returns the number of rows within this matrix.
         */
        auto rows() const -> decltype(rows(m))  {
            using raster::rows;
            return rows(m);
        }
        
        /**
         * Returns the number of colums within this matrix.
         */
        auto columns() const -> decltype(columns(m)) {
            using raster::columns;
            return columns(m);
        }
    };
    
    /**
     * Constructs a translated matrix from the given matrix and a translation function.
     * 
     * @param f The translation function
     * @param m The matrix to be translated by f
     */
    template <typename F, typename M>
    translation<F,M> translated(F &&f, M &&m) {
        return translation<F,M>(std::forward<F>(f), std::forward<M>(m));
    }
}

#endif /* #ifndef RASTER_TRANSLATE_HH */
