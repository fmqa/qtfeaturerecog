/**
 * 
 * Sobel Operator test case.
 * 
 * Compile with g++ -std=c++11 -Wall sobeltest.cc
 * 
 * 
 */

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <utility>
#include <cmath>
#include "pnm.h"
#include "raster/sobel.h"

static float qhypot (float a, float b) {
    return std::sqrt(a * a + b * b);
}

static float qhypot (int a, int b) {
    return std::sqrt(a * a + b * b);
}

int main (int argc, char *argv[]) {
    char type;
    size_t width, height, maxcolors;
    std::ifstream ppmf("input.ppm");
    std::vector<char> rgbv;
    
    pnm::readpnm (ppmf, [&](char t, size_t w, size_t h, size_t mc) {
                            type = t;
                            width = w;
                            height = h;
                            maxcolors = mc;
                            rgbv.reserve (w * h * 3);
                            return std::make_pair(true, std::back_inserter(rgbv));
                        });
    
    std::cout << "PNM type: " << type << ", " 
              << "width: " << width << ", " 
              << "height: " << height << ", " 
              << "max. component: " << maxcolors << std::endl;
    
    std::vector<char> greyv(width * height);
    
    auto fn = [&rgbv,width,height](int x, int y) { 
        if (x >= 0 && y >= 0 && static_cast<unsigned int>(x) < width && static_cast<unsigned int>(y) < height) {
            // Quick, Ad-hoc greyscale conversion
            return (rgbv[3 * (y * width + x)] + 
                    rgbv[3 * (y * width + x) + 1] +
                    rgbv[3 * (y * width + x) + 2]) / 3;
        } else {
            return 0;
        }
    };
    
    rast::sobel_sequence<int,decltype(fn)> seq(width, height, fn);
    
    std::cout << greyv.size() << std::endl;
    
    int i = 0;
    for (auto const &p : seq) {
        greyv[i] = qhypot(p.energy.first, p.energy.second);
        std::cout << i << " : " << p.x << ' ' << p.y << " = " << (int)greyv[i] << std::endl;
        ++i;
    }
    
    std::ofstream pgmf("output.pgm", std::ostream::trunc);
    
    pnm::writepnm(pgmf, '5', width, height, 255, greyv.begin(), greyv.end());
    return 0;
}

