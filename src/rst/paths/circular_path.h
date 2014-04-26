#ifndef CIRCULAR_PATH_H
#define CIRCULAR_PATH_H

#include <utility>
#define _USE_MATH_DEFINES
#include <cmath>

namespace rst {
    template <typename AngleIterator, typename T>
    class circular_path {
    public:
        class iterator : public std::iterator<std::random_access_iterator_tag, std::pair<T,T>> {
            friend class circular_path;
            iterator (AngleIterator a, const circular_path *p) : angle(a), parent(p),
                cursor(calcx(),calcy()) {}
            
            T calcx() const { return parent->x0 + parent->radius * cos(*angle * M_PI / 180); }
            T calcy() const { return parent->y0 + parent->radius * sin(*angle * M_PI / 180); }
            
            void update() {
                cursor.first = calcx();
                cursor.second = calcy();
            }
        public:
            const std::pair<T,T> &operator* () const {
                return cursor;
            }
            
            iterator &operator++ () {
                ++angle;
                update();
                return *this;
            }
            
            iterator operator++ (int) { 
                iterator cpy = *this; 
                ++angle; 
                update();
                return cpy;
            }
            
            iterator &operator-- () {
                --angle;
                update();
                return *this;
            }
            
            iterator operator-- (int) {
                iterator cpy = *this; 
                --angle; 
                update();
                return cpy;
            }
            
            iterator operator+ (typename iterator::difference_type n) const {
                iterator cpy = *this;
                std::advance(cpy.angle, n);
                cpy.update();                
                return cpy;
            }
            
            iterator operator- (typename iterator::difference_type n) const {
                iterator cpy = *this;
                std::advance(cpy.angle, -n);
                cpy.update();
                return cpy;
            }
            
            iterator operator- (const iterator &it) const {
                iterator cpy = *this;
                std::advance(cpy.angle, -it.angle);
                cpy.update();
                return cpy;
            }
            
            iterator &operator+= (typename iterator::difference_type n) {
                std::advance(angle, n);
                update();
                return *this;
            }
            
            iterator &operator-= (typename iterator::difference_type n) {
                std::advance(angle, -n);
                update();
                return *this;
            }
            
            const std::pair<T,T>& operator[] (typename iterator::difference_type n) const {
                iterator cpy = *this;
                std::advance(cpy.angle, n);
                cpy.update();
                return *cpy;
            }
            
            bool operator< (const iterator &it) const { return angle < it.angle; }
            bool operator> (const iterator &it) const { return angle > it.angle; }
            bool operator<= (const iterator &it) const { return angle <= it.angle; }
            bool operator>= (const iterator &it) const { return angle >= it.angle; }            
            bool operator== (const iterator &it) const { return angle == it.angle; }
            bool operator!= (const iterator &it) const { return angle != it.angle; }
        private:
            AngleIterator angle;
            const circular_path *parent;
            std::pair<T,T> cursor;
        };
        
        circular_path (AngleIterator begin, AngleIterator end, 
                       T r, T x = T(), T y = T()) 
            : beginangle(begin),
              endangle(end),
              radius(r),
              x0(x), y0(y) {}
                                                          
        
        iterator begin() const { return iterator(beginangle, this); }
        iterator cbegin() const { return begin(); }
        iterator end() const { return iterator(endangle, this); }
        iterator cend() const { return end; }
    private:
        AngleIterator beginangle, endangle;
        T radius, x0, y0;
    };
    
    template <typename T, typename AngleIterator>
    circular_path<AngleIterator, T> make_circular_path(AngleIterator begin, 
                                                       AngleIterator end,
                                                       T radius, 
                                                       T x = T(), T y = T())
    {
        return circular_path<AngleIterator, T>(begin, end, radius, x, y);
    }
}

#endif /* #ifndef CIRCULAR_PATH_H */
