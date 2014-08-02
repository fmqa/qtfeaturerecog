#ifndef RASTER_TRAITS_HH
#define RASTER_TRAITS_HH

/**
 * Provides traits for querying types implementing the Matrix concept.
 */

#include <type_traits>
#include <utility>
#include "types.hh"

namespace raster {
    /**
     * Traits class for types implementing the Matrix concept.
     * 
     * @tparam T A type fullfilling the Matrix contract
     */
    template <typename T>
    struct traits {
        /**
         * An integral type representing a row count of a matrix.
         */
        typedef decltype(rows(std::declval<T>())) row_type;
        
        /**
         * An integral type representing a column count of a matrix.
         */
        typedef decltype(columns(std::declval<T>())) column_type;
        
        /**
         * The type of the matrix's cell values.
         */
        typedef typename std::decay<decltype(get(std::declval<T>(), 
                                                 std::declval<row_type>(), 
                                                 std::declval<column_type>()))>::type value_type;
    };
}

#endif /* #ifndef RASTER_TRAITS_HH */
