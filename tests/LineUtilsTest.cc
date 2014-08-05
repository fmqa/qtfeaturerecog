#include <array>
#include <gtest/gtest.h>
#include <util/lines.hh>

TEST(LineUtilsTest, TangentFinderTest) {
    std::array<std::array<bool,5>,5> bitmap = {{
        {false,false,false,false,true},
        {false,false,false,true,false},
        {false,false,true,false,false},
        {false,true,false,false,false},
        {true,false,false,false,false}
    }};
    double m = 0, b = 0;
    EXPECT_FALSE(lines::tangent(0, 0, m, b, [&](int x, int y){return bitmap[y][x];}, 2));
    m = 0; b = 0;
    EXPECT_TRUE(lines::tangent(3, 1, m, b, [&](int x, int y){return bitmap[y][x];}, 2));
    EXPECT_TRUE(m >= -0.3 && m <= -0.1);
    EXPECT_TRUE(b >= 1 && b <= 2);
}

TEST(LineUtilsTest, IntersectionTest) {
    double x, y;
    
    EXPECT_TRUE(lines::intersect(0, 0, 10, 10,
                                 10, 0, 0, 10, x, y));
    EXPECT_TRUE(x >= 4.8 && x <= 5.1);
    EXPECT_TRUE(y >= 4.8 && y <= 5.1);
}
