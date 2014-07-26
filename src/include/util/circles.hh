#ifndef CIRCLES_HH
#define CIRCLES_HH

#include <cmath>
#include <iterator>
#include <functional>

namespace circles {
    extern const std::array<double,360> unit;
    
    template <typename T, 
              typename InputIterator, 
              typename OutputIterator, 
              typename U = int>
    OutputIterator trigcircle(T radius, 
                              InputIterator firstangle,
                              InputIterator lastangle,
                              OutputIterator out,
                              U x0 = U(),
                              U y0 = U())
    {
        for (; firstangle != lastangle; ++firstangle) {
            *out++ = std::make_pair(x0 + radius * std::cos(*firstangle),
                                    y0 + radius * std::sin(*firstangle));
        }
        return out;
    }
    
    template <typename ForwardIterator>
    class trigonometric {
        ForwardIterator begin, end;
    public:
        trigonometric(ForwardIterator first, ForwardIterator last) 
            : begin(first), 
              end(last) {}
        
        template <typename T, 
                  typename OutputIterator, 
                  typename U = int>
        OutputIterator operator()(T radius, 
                                  OutputIterator out, 
                                  U x0 = U(), 
                                  U y0 = U()) 
        {
            return trigcircle(radius, begin, end, out, x0, y0);
        }
    };
    
    template <typename ForwardIterator>
    trigonometric<ForwardIterator> trig(ForwardIterator begin, 
                                        ForwardIterator end) 
    {
        return trigonometric<ForwardIterator>(begin, end);
    }
    
    template <typename OutputIterator>
    OutputIterator brescircle(int radius, 
                              OutputIterator out, 
                              int x0 = 0, 
                              int y0 = 0) 
    {
        int x = radius;
        int y = 0;
        int radiuserr = 1 - x;
        
        while (x >= y) {
            *out++ = std::make_pair(x + x0, y + y0);
            *out++ = std::make_pair(y + x0, x + y0);
            *out++ = std::make_pair(-x + x0, y + y0);
            *out++ = std::make_pair(-y + x0, x + y0);
            *out++ = std::make_pair(-x + x0, -y + y0);
            *out++ = std::make_pair(-y + x0, -x + y0);
            *out++ = std::make_pair(x + x0, -y + y0);
            *out++ = std::make_pair(y + x0, -x + y0);
            ++y;
            if (radiuserr < 0) {
                radiuserr += 2 * y + 1;
            } else {
                --x;
                radiuserr += 2 * (y - x + 1);
            }
        }
        return out;
    }
    
    class bres {
    public:
        template <typename OutputIterator>
        OutputIterator operator()(int radius, 
                                  OutputIterator out, 
                                  int x0 = 0, 
                                  int y0 = 0) 
        {
            return brescircle(radius, out, x0, y0);
        }
    };
    
    template <typename T, 
              typename InputIterator, 
              typename OutputIterator,
              typename Circle,
              typename X,
              typename Y>
    OutputIterator hough(T radius,
                         InputIterator firstpoint,
                         InputIterator lastpoint,
                         OutputIterator out,
                         Circle &&circle,
                         X &&x,
                         Y &&y)
    {
        for (; firstpoint != lastpoint; ++firstpoint) {
            circle(radius, out, x(*firstpoint), y(*firstpoint));
        }
        return out;
    }
}

#endif /* #ifndef CIRCLES_HH */
