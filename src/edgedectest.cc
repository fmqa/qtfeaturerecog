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
#include <vector>
#include "pnm.h"
#include "rst/filter/gaussian.h"
#include "rst/filter/canny.h"
#include "rst/filter/wrap.h"

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Insufficient argument count" << std::endl;
        std::cerr << "\tSyntax: " << argv[0] << " <PPM-INPUT> <PGM-OUTPUT> " << std::endl;
        return 1;
    }
    char type;
    size_t width, height, maxcolors;
    std::ifstream ppmf(argv[1]);
    std::vector<unsigned char> rgbv;
    
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
    
    std::vector<unsigned char> greyv(width * height);
    
    auto fn = [width,height,&rgbv](int y, int x) {
        return (rgbv[3 * (y * width + x)] +
                rgbv[3 * (y * width + x) + 1] +
                rgbv[3 * (y * width + x) + 2]) / 3;
    };
    
    // Edge-Extending wrapper
    auto wrap = rst::make_ext_wrapper(fn, (int)height, (int)width);
    rst::gaussian_filter<5> gaussian(1.3);
    
    rst::canny([&wrap,&gaussian](int y, int x) {                   
                   return gaussian.apply(wrap, y, x);
               }, 
               [&greyv,height,width](int y, int x) -> unsigned char& {
                   return greyv[y * width + x];
               }, 
               [](const std::pair<double,double> &ab) {
                   return sqrt(ab.first*ab.first+ab.second*ab.second);
               }, 
               height, width, 1, 20);

    
    std::ofstream pgmf(argv[2], std::ostream::trunc);
    
    pnm::writepnm(pgmf, '5', width, height, 255, greyv.begin(), greyv.end());
    return 0;
}


