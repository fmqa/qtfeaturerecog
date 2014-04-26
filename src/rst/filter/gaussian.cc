#include <cmath>
#include "gaussian.h"

double rst::gaussian(double y, double x, double sigma) {
    return exp(-((x * x + y * y) / (2 * sigma * sigma))) / (2 * sigma * sigma);
}
