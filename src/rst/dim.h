#ifndef RST_DIM_H
#define RST_DIM_H

#include <utility>

namespace rst {
    template <typename T>
    auto rows (T &&t) -> decltype(std::forward<T>(t).rows()) {
        return std::forward<T>(t).rows();
    }
    
    template <typename T>
    auto columns (T &&t) -> decltype(std::forward<T>(t).columns()) {
        return std::forward<T>(t).columns();
    }
}

#endif /* #ifndef RST_DIM_H */
