#ifndef RASTER_TRAITS_HH
#define RASTER_TRAITS_HH

#include <type_traits>
#include <utility>
#include "types.hh"

namespace raster {
    template <typename T>
    struct traits {
        typedef decltype(rows(std::declval<T>())) row_type;
        typedef decltype(columns(std::declval<T>())) column_type;
        typedef typename std::decay<decltype(get(std::declval<T>(), 
                                                 std::declval<row_type>(), 
                                                 std::declval<column_type>()))>::type value_type;
    };
}

#endif /* #ifndef RASTER_TRAITS_HH */
