#ifndef CIRCLES_HH
#define CIRCLES_HH

/**
 * Plotting and Hough transformation algorithms for circles.
 */

#include <cmath>
#include <iterator>
#include <functional>

namespace circles {
    /**
     * Unit circle (r=1U) angles in radians.
     */
    extern const std::array<double,360> unit;
    
    /**
     * Aggregate containing the parameters of a circle.
     * 
     * @tparam T x/y center coordinate type
     * @tparam U radius type
     */
    template <typename T, typename U = T>
    struct parametric_circle {
        T x, y;
        U radius;
    };
    
    /**
     * Returns true if a given point falls within the area of a given circle,
     * false otherwise.
     * 
     * @param x The x-coordinate of the point to test
     * @param y The y-coordinate of the point to test
     * @param cx The x-coordinate of the circle's center
     * @param cy The y-coordinate of the circle's center
     * @param The radius of the circle
     * @return true or false, depending on whether the point (x,y) falls within the circle
     * or not, respectively
     */
    template <typename T, typename U, typename V, typename W, typename X>
    bool within(T x, U y, V cx, W cy, X r) {
        return ((x - cx) * (x - cx) + (y - cy) * (y - cy)) < (r * r);
    }
    
    /**
     * Returns true if a given point falls within the area of a given circle,
     * false otherwise.
     *
     * @param x The x-coordinate of the point to test
     * @param y The y-coordinate of the point to test
     * @param pc The parameters of the circle
     * @return true or false, depending on whether the point (x,y) falls within the circle
     * or not, respectively
     */
    template <typename T, typename U, typename V, typename W>
    bool within(T x, U y, const parametric_circle<V,W> &pc) {
        return within(x, y, pc.x, pc.y, pc.radius);
    }
    
    /**
     * Generate a circle using trigonometric functions.
     * 
     * This function outputs the (x,y) pairs denoting a circle's edges
     * to the given output iterator.
     * 
     * @param radius Radius of the circle to generate
     * @param firstangle Iterator pointing to the angle of the circle to begin from
     * @param lastangle Iterator pointing to the last angle of the circle
     * @param out Output iterator where the (x,y)-pairs will be written to
     * @param x0 The x-coordinate of the center point of the circle to be generated
     * @param y0 The y-coordinate of the center point of the circle to be generated
     * @return The output iterator out
     */
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
    
    /**
     * Trigonometric circle generator (Functor).
     * 
     * @tparam ForwardIterator The type of the angle iterator
     */
    template <typename ForwardIterator>
    class trigonometric {
        ForwardIterator begin, end;
    public:
        /**
         * Creates a trigonometric circle generator, generating circles 
         * which begin from the angle first and end at the angle last.
         * 
         * @param first Iterator to the circle angle to begin from
         * @param last Iterator the circle angle to stop at
         */
        trigonometric(ForwardIterator first, ForwardIterator last) 
            : begin(first), 
              end(last) {}
              
        /**
         * Generates a circle with the given radius and center point.
         * 
         * @param radius The radius of the circle to be generated
         * @param out The iterator where the circle's edges will be written
         * @param x0 The x-coordinate of the center point of the circle to be generated
         * @param y0 The y-coordinate of the center point of the circle to be generated
         * @return The output iterator out
         */
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
    
    /**
     * Factory function for trigonometric circle functors.
     * 
     * @param begin Iterator to the circle angle to begin from
     * @param end Iterator the circle angle to stop at
     * @return A trigonometric circle generator functor
     */
    template <typename ForwardIterator>
    trigonometric<ForwardIterator> trig(ForwardIterator begin, 
                                        ForwardIterator end) 
    {
        return trigonometric<ForwardIterator>(begin, end);
    }
    
    /**
     * Generate a circle using Bresenham's circle plotting algorithm.
     * 
     * @param radius The radius of the circle to be generated
     * @param out The output iterator when the edges of the circle will be written to
     * @param x0 The x-coordinate of the center point of the circle to be generated
     * @param y0 The y-coordinate of the center point of the circle to be generated
     * @return The output iterator out
     */
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
    
    /**
     * Functor wrapper for the Bresenham circle generator.
     */
    class bres {
    public:
        template <typename OutputIterator>
        OutputIterator operator()(int radius, 
                                  OutputIterator out, 
                                  int x0 = 0, 
                                  int y0 = 0) const
        {
            return brescircle(radius, out, x0, y0);
        }
    };
    
    /**
     * Apply a Hough transform to a sequence of points.
     * 
     * @param radius The circle radius to test for
     * @param firstpoint Iterator to the first point in the point sequence
     * @param lastpoint Iterator to the last point in the point sequence
     * @param out Iterator when all points of candidate circles will be placed
     * @param circle The circle plotting function to be used
     * @param x The x-coordinate accessor function
     * @param y The y-coordinate accessor function
     * @return The output iterator out
     */
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
