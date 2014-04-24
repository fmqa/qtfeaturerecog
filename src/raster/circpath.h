#ifndef CIRCPATH_H
#define CIRCPATH_H

#include <utility>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../ranges/interval.h"

namespace rast {
    template <typename AngleIterator, typename T>
    class circpath {
    public:
        class iterator : public std::iterator<std::random_access_iterator_tag, std::pair<T,T>> {
            friend class circpath;
            iterator (AngleIterator a, const circpath *p) : angle(a), parent(p),
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
            const circpath *parent;
            std::pair<T,T> cursor;
        };
        
        circpath (AngleIterator begin, AngleIterator end, 
                  T r, T x = T(), T y = T()) : beginangle(begin), 
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
    circpath<AngleIterator, T> makecircpath(AngleIterator begin, AngleIterator end, 
                                            T radius, T x = T(), T y = T()) 
    {
        return circpath<AngleIterator, T>(begin, end, radius, x, y);
    }
    
    template <typename T>
    circpath<T,ranges::interval<int>> makestdcircpath(T radius, T x = T(), T y = T()) {
        ranges::interval<int> fullarc(0, 359, 1);
        return makecircpath(fullarc.begin(), fullarc.end(), x, y);
    }
}

#endif /* #ifndef CIRCPATH_H */
