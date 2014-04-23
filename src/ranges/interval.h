#ifndef INTERVAL_H
#define INTERVAL_H

#include <iterator>

namespace ranges {
    template <typename T>
    class interval {
    public:
        class iterator : public std::iterator<std::random_access_iterator_tag, T> {
            friend class interval;
            iterator (const interval *p, T val) : parent(p), value(val) {}
        public:
            const T& operator* () const { return value; }
            
            iterator &operator++ () {
                if (parent) {
                    T update = value + parent->step;
                    if (update >= value && update <= parent->max) {
                        value = update;
                    } else {
                        value = T();
                        parent = 0;
                    }
                }
                return *this;
            }
            
            iterator operator++ (int) {
                iterator cpy = *this;
                if (parent) {
                    T update = value + parent->step;
                    if (update >= value && update <= parent->max) {
                        value = update;
                    } else {
                        value = T();
                        parent = 0;
                    }
                }
                return cpy;
            }
            
            iterator &operator-- () {
                if (parent) {
                    T update = value - parent->step;
                    if (update <= value && update >= parent->min) {
                        value = update;
                    } else {
                        value = T();
                        parent = 0;
                    }
                }
                return *this;
            }
            
            iterator operator-- (int) {
                iterator cpy = *this;
                if (parent) {
                    T update = value - parent->step;
                    if (update <= value && update >= parent->min) {
                        value = update;
                    } else {
                        value = T();
                        parent = 0;
                    }
                }
                return cpy;
            }
            
            iterator operator+ (typename iterator::difference_type n) const {
                iterator cpy = *this;
                if (parent) {
                    cpy.value += n * parent->step;
                    if (cpy.value < value || cpy.value > parent->max) {
                        cpy.value = T();
                        cpy.parent = 0;
                    }
                }
                return cpy;
            }
            
            iterator operator- (typename iterator::difference_type n) const {
                iterator cpy = *this;
                if (parent) {
                    cpy.value -= n * parent->step;
                    if (cpy.value > value || cpy.value < parent->min) {
                        cpy.value = T();
                        cpy.parent = 0;
                    }
                }
                return cpy;
            }
            
            iterator operator- (const iterator &it) const {
                iterator cpy = *this;
                if (parent) {
                    cpy.value -= it.value;
                    if (cpy.value > value || cpy.value < parent->min) {
                        cpy.value = T();
                        cpy.parent = 0;
                    }
                }
                return cpy;
            }
            
            iterator &operator+= (typename iterator::difference_type n) {
                if (parent) {
                    T update = value + n * parent->step;
                    if (update >= value && update <= parent->max) {
                        value = update;
                    } else {
                        value = T();
                        parent = 0;
                    }
                }
                return *this;
            }
            
            iterator &operator-= (typename iterator::difference_type n) {
                if (parent) {
                    T update = value - n * parent->step;
                    if (update <= value && update >= parent->min) {
                        value = update;
                    } else {
                        value = T();
                        parent = 0;
                    }
                }
                return *this;
            }
            
            const T& operator[] (typename iterator::difference_type n) const {
                iterator cpy = *this;
                cpy += n;
                return cpy;
            }
            
            bool operator< (const iterator &it) const { return it.parent == parent && it.value < value; }
            bool operator> (const iterator &it) const { return it.parent == parent && it.value > value; }
            bool operator<= (const iterator &it) const { return it.parent == parent && it.value <= value; }
            bool operator>= (const iterator &it) const { return it.parent == parent && it.value >= value; }
            bool operator== (const iterator &it) const { return it.parent == parent && it.value == value; }
            bool operator!= (const iterator &it) const { return it.parent != parent || it.value != value; }
        private:
            const interval *parent;
            T value;
        };
        
        interval (T first, T last, T st) : min(first), max(last), step(st) {}
        iterator begin() const { return iterator(this, min); }
        iterator end() const { return iterator(0, T()); }
        iterator cbegin() const { return begin(); }
        iterator cend() const { return end(); }
    private:
        T min, max, step;
    };
}

#endif /* #ifndef INTERVAL_H */
