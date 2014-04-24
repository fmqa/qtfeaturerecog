#ifndef SOBEL_H 
#define SOBEL_H

#include <utility>
#include <iterator>

namespace rast {
    template <typename F, typename T>
    auto sobel (F f, T x = T(), T y = T()) -> std::pair<decltype(f(x,y)),decltype(f(x,y))> {
        return std::make_pair(
            -1 * f(x-1, y-1) +  1 * f(x+1, y-1) +
            -2 * f(x-1, y)   +  2 * f(x+1, y) +
            -1 * f(x-1, y+1) +  1 * f(x+1, y+1),
             1 * f(x-1, y-1) +  2 * f(x, y-1) +  1 * f(x+1, y-1) +
            -1 * f(x-1, y+1) + -2 * f(x, y+1) + -1 * f(x+1, y+1)
        );
    }
    
    template <typename T, typename F>
    using sobelresult_t = decltype(sobel<F,T>(std::declval<F>()));
    
    template <typename T, typename F>
    struct sobel_edge {
        sobel_edge (T x = T(), 
                    T y = T(),
                    sobelresult_t<T,F> nrg = sobelresult_t<T,F>()) 
        : x(x), y(y), energy(nrg) {}
        T x;
        T y;
        sobelresult_t<T,F> energy;
    };
    
    template <typename T, typename F>
    class sobel_sequence {
        static_assert(std::is_integral<T>::value && std::is_signed<T>::value,
                      "Index type must be a signed integer type");
    public:
        class iterator : public std::iterator<std::bidirectional_iterator_tag,
                                              sobel_edge<T,F>> 
        {
            friend class sobel_sequence;
            
            iterator (const sobel_sequence *p, sobel_edge<T,F> v) : parent(p), value(v) {}
            
            void advance () {
                ++value.x;
                if (value.x >= parent->width) {
                    ++value.y;
                    if (value.y >= parent->height) {
                        --value.y;
                    } else {
                        value.x = 0;
                    }
                }
            }

            void backtrack () {
                --value.x;
                if (value.x < 0) {
                    --value.y;
                    if (value.y < 0) {
                        ++value.y;
                    } else {
                        value.x = parent->width - 1;
                    }
                }
            }
         
            void update () {
                value.energy = sobel(parent->fn, value.x, value.y);
            }
        public:
            const sobel_edge<T,F>& operator* () const { return value; }
            
            iterator& operator++ () {
                if (parent) {
                    advance();
                    update();
                }
                return *this;
            }
            
            iterator operator++ (int) {
                iterator cpy = *this;
                 if (parent) {
                    update();
                    advance();
                }
                return cpy;
            }
            
            iterator& operator-- () {
                if (parent) {
                    backtrack();
                    update();
                }
                return *this;
            }
            
            iterator operator-- (int) {
                iterator cpy = *this;
                if (parent) {
                    backtrack();
                    update();
                }
                return cpy;
            }
            
            bool operator== (const iterator &it) const { 
                return parent == it.parent
                    && value.x == it.value.x
                    && value.y == it.value.y;
            }
            
            bool operator!= (const iterator &it) const {
                return parent != it.parent
                    || value.x != it.value.x
                    || value.y != it.value.y;
            }
        private:
            const sobel_sequence *parent;
            sobel_edge<T,F> value;
        };
        
        sobel_sequence (T wdt, T hgt, F f) : width(wdt), height(hgt), fn(f) {}
        
        iterator begin() const { 
            return iterator(this, sobel_edge<T,F>(T(), T(), sobel<F,T>(fn)));
        }
        iterator end() const { 
            return iterator(this, 
                            sobel_edge<T,F>(width, height - 1, 
                                            sobelresult_t<T,F>()));
            
        }
        iterator cbegin() const { return begin(); }
        iterator cend() const { return end(); }
    private:
        T width, height;
        F fn;
    };
}

#endif /* #ifndef SOBEL_H */
