#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <raster/flt/sobel.hh>
#include <raster/flt/nms.hh>
#include <raster/flt/gaussian.hh>

TEST(RasterFilterTest, SobelTest) {
    std::array<std::array<int,3>,3> mat = {{
        {0,1,2},
        {3,4,5},
        {6,7,8}
    }};
    std::pair<int,int> gradients = raster::fn::sobel(mat, 1, 1);
    EXPECT_EQ(gradients.first, 0 * 1 + 1 * 2 + 2 * 1 - 6 * 1 - 7 * 2 - 1 * 8);
    EXPECT_EQ(gradients.second, -1 * 0 - 2 * 3 - 1 * 6 + 1 * 2 + 2 * 5 + 1 * 8);
}

TEST(RasterFilterTest, NMSTest) {
    std::array<std::array<int,3>,3> mat = {{
        {0,0,1},
        {0,1,0},
        {1,0,0}
    }};
    auto mag = raster::fn::nms(mat, static_cast<double(*)(double,double)>(std::hypot), 1, 1);
    EXPECT_EQ(mag, 0);
}

TEST(RasterFilterTest, GaussianTest) {
    std::array<std::array<double,3>,3> blur;
    raster::krn::gaussian(blur, 1.0);
    EXPECT_TRUE(blur[0][0] >= 0.07 && blur[0][0] <= 0.08);
    EXPECT_TRUE(blur[0][1] >= 0.10 && blur[0][1] <= 0.15);
    EXPECT_TRUE(blur[0][2] >= 0.07 && blur[0][2] <= 0.08);
    EXPECT_TRUE(blur[1][0] >= 0.07 && blur[0][0] <= 0.08);
    EXPECT_TRUE(blur[1][1] >= 0.10 && blur[0][1] <= 0.15);
    EXPECT_TRUE(blur[1][2] >= 0.07 && blur[0][2] <= 0.08);
    EXPECT_TRUE(blur[1][0] >= 0.07 && blur[0][0] <= 0.08);
    EXPECT_TRUE(blur[1][1] >= 0.10 && blur[0][1] <= 0.15);
    EXPECT_TRUE(blur[1][2] >= 0.07 && blur[0][2] <= 0.08);
}
