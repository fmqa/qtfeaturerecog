/**
 * 
 * Console edge detection demo.
 * 
 * Compile with g++ -std=c++11 -Wall sobeltest.cc rst/filter/gaussian.cc rst/filter/sobel.cc
 * 
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "pnm.h"
#include "edgedec.h"

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
    
    auto infn = [width,height,&rgbv](int y, int x) {
        return (rgbv[3 * (y * width + x)] +
                rgbv[3 * (y * width + x) + 1] +
                rgbv[3 * (y * width + x) + 2]) / 3;
    };
    
    auto outfn = [&greyv,height,width](int y, int x) -> unsigned char& {
        return greyv[y * width + x];
    };
    
    auto ed = fr::make_edgedec(infn, outfn, height, width, 1, 30);
    
    ed.apply ();
    
    std::ofstream pgmf(argv[2], std::ostream::trunc);
    
    pnm::writepnm(pgmf, '5', width, height, 255, greyv.begin(), greyv.end());
    return 0;
}


