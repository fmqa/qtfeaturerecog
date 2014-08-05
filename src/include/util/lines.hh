#ifndef LINES_HH
#define LINES_HH

#include <cmath>

/**
 * Algorithms for lines and linear functions.
 */

namespace lines {
    /**
     * Finds the tangent passing through the point (x,y) within an image.
     * 
     * @param x The x-coordinate of the tangent point
     * @param y The y-coordinate of the tangent point
     * @param m Output parameter where the slope of the tangent will be stored
     * @param b Output parameter where the y-axis offset of the tangent will be stored
     * @param f The monochrome image predicate containing the image points
     * @param off The block size to be used for the linear regression
     * @return true if a tangent passes through the given point, false otherwise
     */
    template <typename T, typename U, typename V, typename F>
    bool tangent(T x, T y, U &m, V &b, F &&f, int off = 2) {
        if (!f(x, y)) {
            return false;
        }
        
        int n = 0, sx = 0, sy = 0, sxy = 0, sxx = 0;
        
        for (int offy = -off; offy <= off; ++offy) {
            for (int offx = -off; offx <= off; ++offx) {
                if (f(x + offx, y + offy)) {
                    sx += x + offx;
                    sy += y + offy;
                    sxy += (x + offx) * (y + offy);
                    sxx += (x + offx) * (x + offx);
                    ++n;
                }
            }
        }
        
        
        if (n) {
            m = (n * sxy - sx * sy) / static_cast<double>(n * sxx - sx * sx);
            b = (sy / n) - (m * sx) / static_cast<double>(n);
            return true;
        }
        
        return false;
    }
    
    /**
     * Finds the intersection point of the 2 tangents passing through the given point within
     * an image.
     * 
     * @param x0 The x-coordinate of the first tangent point
     * @param y0 The y-coordinate of the first tangent point
     * @param x1 The x-coordinate of the first tangent point
     * @param y1 The y-coordinate of the first tangent point
     * @param xt Output parameter where the x-coordinate of the intersection point will be stored
     * @param yt Output parameter where the y-coordinate of the intersection point will be stored
     * @param f The monochrome image predicate containing the image points
     * @param off The block size to be used for the linear regression
     * @return true if an intersection point exists, false otherwise
     */
    template <typename T, typename U, typename V, typename F>
    bool tangent_intersection(T x0, T y0, U x1, U y1, V &xt, V &yt, F &&f, int off = 2) {
        double m0, b0, m1, b1;
        
        if (!tangent(x0, y0, m0, b0, f, off) || !tangent(x1, y1, m1, b1, f, off) || m0 == m1) {
            return false;
        }
        
        if (std::isfinite(m0) && std::isfinite(m1)) {
            xt = (b1 - b0) / (m0 - m1);
            yt = m0 * xt + b0;
            return true;
        }
        
        if (std::isfinite(m0) && !std::isfinite(m1)) {
            xt = x1;
            yt = m0 * xt + b0;
            return true;
        }
        
        if (!std::isfinite(m0) && std::isfinite(m1)) {
            xt = x0;
            yt = m1 * xt + b1;
            return true;
        }
        
        return false;
    }
    
    /**
     * Finds the intersection point of 2 given lines.
     * 
     * @param x0 The x-coordinate of the first line's origin point
     * @param y0 The y-coordinate of the first line's origin point
     * @param x1 The x-coordinate of the first line's destination point
     * @param y1 The y-coordinate of the first line's destination point
     * @param x2 The x-coordinate of the second line's origin point
     * @param y2 The y-coordinate of the second line's origin point
     * @param x3 The x-coordinate of the second line's destination point
     * @param y4 The y-coordinate of the second line's destination point
     * @param xi Output parameter where the x-coordinate of the intersection point will be stored
     * @param yi Output parameter where the y-coordinate of the intersection point will be stored
     * @return true if the given lines intersect, false otherwise
     */
    template <typename T, typename U, typename V, typename W, typename X>
    bool intersect(T x0, T y0, 
                   U x1, U y1, 
                   V x2, V y2,
                   W x3, W y3,
                   X &xi, X &yi) 
    {
        double m01 = (y1 - y0) / static_cast<double>(x1 - x0);
        double b01 = y0 - m01 * x0;
        
        double m23 = (y3 - y2) / static_cast<double>(x3 - x2);
        double b23 = y2 - m23 * x2;
        
        if (m01 == m23) {
            return false;
        }
        
        if (std::isfinite(m01) && std::isfinite(m23)) {
            xi = (b23 - b01) / (m01 - m23);
            yi = m01 * xi + b01;
            return true;
        }
        
        if (std::isfinite(m01) && !std::isfinite(m23)) {
            xi = x2;
            yi = m01 * xi + b01;
            return true;
        }
        
        if (!std::isfinite(m01) && std::isfinite(m23)) {
            xi = x0;
            yi = m23 * xi + b23;
            return true;
        }
        
        return false;
    }
}

#endif /* #ifndef LINES_HH */
