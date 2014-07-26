#include <cmath>
#include "gaussian.hh"

double raster::fn::gaussian(double y, double x, double sigma) {
    return exp(-((x * x + y * y) / (2 * sigma * sigma))) / (2 * sigma * sigma);
}
