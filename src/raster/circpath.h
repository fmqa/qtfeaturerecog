#ifndef CIRCPATH_H
#define CIRCPATH_H

#include <utility>
#include <cmath>

namespace raster {
    template <typename AngleIterator, typename T>
    class circpath {
    public:
        class iterator : public std::iterator<std::random_access_iterator_tag, std::pair<T,T>> {
            friend class circpath;
            iterator (AngleIterator a, const circpath *p) : angle(a), parent(p),
                cursor(parent->x0 + parent->radius * cos(*angle),
                       parent->y0 + parent->radius * sin(*angle)) {}
        public:
            const std::pair<T,T> &operator* () const {
                return cursor;
            }
            
            iterator &operator++ () {
                ++angle;
                cursor.first = parent->x0 + parent->radius * cos(*angle);
                cursor.second = parent->y0 + parent->radius * sin(*angle);
                return *this;
            }
            
            iterator operator++ (int) { 
                iterator cpy = *this; 
                ++angle; 
                cursor.first = parent->x0 + parent->radius * cos(*angle);
                cursor.second = parent->y0 + parent->radius * sin(*angle);
                return cpy;
            }
            
            iterator &operator-- () {
                --angle;
                cursor.first = parent->x0 + parent->radius * cos(*angle);
                cursor.second = parent->y0 + parent->radius * sin(*angle);
                return *this;
            }
            
            iterator operator-- (int) {
                iterator cpy = *this; 
                --angle; 
                cursor.first = parent->x0 + parent->radius * cos(*angle);
                cursor.second = parent->y0 + parent->radius * sin(*angle);
                return cpy;
            }
            
            iterator operator+ (typename iterator::difference_type n) const {
                iterator cpy = *this;
                std::advance(cpy.angle, n);
                cpy.cursor.first = parent->x0 + parent->radius * cos(*angle);
                cpy.cursor.second = parent->y0 + parent->radius * sin(*angle);
                return cpy;
            }
            
            iterator operator- (typename iterator::difference_type n) const {
                iterator cpy = *this;
                std::advance(cpy.angle, -n);
                cpy.cursor.first = parent->x0 + parent->radius * cos(*angle);
                cpy.cursor.second = parent->y0 + parent->radius * sin(*angle);
                return cpy;
            }
            
            iterator operator- (const iterator &it) const {
                iterator cpy = *this;
                std::advance(cpy.angle, -it.angle);
                cpy.cursor.first = parent->x0 + parent->radius * cos(*angle);
                cpy.cursor.second = parent->y0 + parent->radius * sin(*angle);
                return cpy;
            }
            
            iterator &operator+= (typename iterator::difference_type n) {
                std::advance(angle, n);
                cursor.first = parent->x0 + parent->radius * cos(*angle);
                cursor.second = parent->y0 + parent->radius * sin(*angle);
                return *this;
            }
            
            iterator &operator-= (typename iterator::difference_type n) {
                std::advance(angle, -n);
                cursor.first = parent->x0 + parent->radius * cos(*angle);
                cursor.second = parent->y0 + parent->radius * sin(*angle);
                return *this;
            }
            
            const std::pair<T,T>& operator[] (typename iterator::difference_type n) const {
                iterator cpy = *this;
                std::advance(cpy.angle, n);
                cpy.cursor.first = parent->x0 + parent->radius * cos(*angle);
                cpy.cursor.second = parent->y0 + parent->radius * sin(*angle);
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
}

#endif /* #ifndef CIRCPATH_H */
