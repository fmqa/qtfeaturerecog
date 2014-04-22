#include <iostream>
#include <vector>
#include "image/ellipfit.h"
#include "monoellip4.xbm"

int main (int argc, char *argv[]) {
    std::vector<std::pair<int,int>> edges;
    
    for (int i = 0; i < (monoellip_width * monoellip_height) / 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (monoellip_bits[i] & (1 << j)) {
                edges.emplace_back(((i * 8) + j) % 64, ((i * 8) + j) / 64);
            }
        }
    }
    
    img::ellipfit (edges.begin(), edges.end(), 21, 17,
                   [](double xc, double yc, double a, int b, float orientation) {
                       std::cout << (int)xc << ' ' 
                                 << (int)yc << ' ' 
                                 << (int)a << ' ' 
                                 << (int)b << ' '
                                 << orientation << std::endl;
                   },
                   8);
    
    return 0;
}

