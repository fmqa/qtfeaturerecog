#ifndef BRESENHAM_CIRCLE_H
#define BRESENHAM_CIRCLE_H

#include <type_traits>
#include <utility>
#include <iterator>

namespace rst {
    extern const int bres_indices[8][2][2];
    
    template <typename T, 
              typename = typename std::enable_if<
                  std::is_integral<T>::value && 
                  std::is_signed<T>::value>::type>
    class bres_circular_path 
        : public std::iterator<std::forward_iterator_tag, std::pair<T,T>> 
    {
    public:
        class iterator {
            friend class bres_circular_path;
            
            iterator (const bres_circular_path *p) 
                : parent(p), x(p->radius), y(), i(), 
                  radiuserr(1 - p->radius), point() 
            {
                ++(*this);
            }
            
            iterator () : parent(), x(), y(), i(), radiuserr(), point() {}
        public:
            const std::pair<T,T>& operator* () const {
                return point;
            }
            
            iterator &operator++ () {
                if (parent) {
                    if (x >= y) {
                        point.first = parent->x0 + 
                                      (bres_indices[i][0][0] * 
                                       (bres_indices[i][0][1] ? y : x));
                        point.second = parent->y0 +
                                       (bres_indices[i][1][0] * 
                                        (bres_indices[i][1][1] ? y : x));
                        ++i;
                        if (i == 8) {
                            i = 0;
                            ++y;
                            if (radiuserr < 0) {
                                radiuserr += 2 * y + 1;
                            } else {
                                --x;
                                radiuserr += 2 * (y - x + 1);
                            }
                        }
                    } else {
                        parent = 0;
                        x = 0;
                        y = 0;
                        i = 0;
                        radiuserr = 0;
                        point.first = 0;
                        point.second = 0;
                    }
                }
                return *this;
            }
            
            iterator operator++ (int) const {
                iterator cpy = *this;
                ++(*this);
                return cpy;
            }
            
            bool operator== (iterator &it) const { 
                return parent == it.parent && point == it.point;
            }
            
            bool operator!= (iterator &it) const {
                return parent != it.parent || point != it.point;
            }
        private:
            const bres_circular_path *parent;
            T x, y;
            T i, radiuserr;
            std::pair<T,T> point;
        };
        
        bres_circular_path (T rad, T x = T(), T y = T()) 
            : radius(rad), x0(x), y0(y) {}
        
        iterator begin() const { return iterator(this); }
        iterator end() const { return iterator(); }
        iterator cbegin() const { return begin(); }
        iterator cend() const { return end(); }
    private:
        T radius, x0, y0;
    };
    
    template <typename T>
    bres_circular_path<T> make_bres_circular_path(T radius,
                                                  T x = T(), T y = T()) 
    {
        return bres_circular_path<T>(radius, x, y);
    }
}

#endif /* #ifndef BRESENHAM_CIRCLE_H */
