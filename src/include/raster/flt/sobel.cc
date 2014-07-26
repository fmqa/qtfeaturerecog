#include "sobel.hh"

const std::array<std::array<int,3>,3> raster::krn::sobel::x = {{{-1,0,1},{-2,0,2},{-1,0,1}}};
const std::array<std::array<int,3>,3> raster::krn::sobel::y = {{{1,2,1},{0,0,0},{-1,-2,-1}}};
