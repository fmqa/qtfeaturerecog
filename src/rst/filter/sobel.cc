#include "sobel.h"

const int rst::sobel_x_kernel[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
const int rst::sobel_y_kernel[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
