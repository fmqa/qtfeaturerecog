#ifndef WRAP_H
#define WRAP_H

namespace rst {
    template <typename F, typename T>
    struct ext_wrapper {
    private:
        F fn;
        T height, width;
    public:
        explicit ext_wrapper(F f, T m, T n) : fn(f), height(m), width(n) {}
        auto operator() (T y, T x) -> decltype(fn(y, x)) {
            if (x >= 0 && x < width) {
                if (y < 0) {
                    return fn(0, x);
                }
                if (y >= height) {
                    return fn(height - 1, x);
                }
            }
            if (y >= 0 && y < height) {
                if (x < 0) {
                    return fn(y, 0);
                }
                if (x >= width) {
                    return fn(y, width - 1);
                }
            }
            if (y < 0) {
                if (x < 0) {
                    return fn(0, 0);
                }
                if (x >= width) {
                    return fn(0, width - 1);
                }
            }
            if (y >= height) {
                if (x < 0) {
                    return fn(height - 1, 0);
                }
                if (x >= width) {
                    return fn(height - 1, width - 1);
                }
            }
            return fn(y, x);
        }
    };
    
    template <typename F, typename T>
    ext_wrapper<F,T> make_ext_wrapper(F f, T m, T n) {
        return ext_wrapper<F,T>(f, m, n);
    }
}

#endif /* #ifndef WRAP_H */
