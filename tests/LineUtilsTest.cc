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
    EXPECT_FALSE(lines::tangent(0, 0, m, b, [&](int x, int y){return x >= 0 && y >= 0 && x < 5 && y < 5 ? bitmap[y][x] : false;}, 2));
    m = 0; b = 0;
    EXPECT_TRUE(lines::tangent(2, 2, m, b, [&](int x, int y){return x >= 0 && y >= 0 && x < 5 && y < 5 ? bitmap[y][x] : false;}, 2));
    EXPECT_TRUE(std::abs(m) >= 0.8 && std::abs(m) <= 1.5);
    EXPECT_TRUE(b >= 3 && b <= 4);
}

TEST(LineUtilsTest, IntersectionTest) {
    double x, y;
    
    EXPECT_TRUE(lines::intersect(0, 0, 10, 10,
                                 10, 0, 0, 10, x, y));
    EXPECT_TRUE(x >= 4.8 && x <= 5.1);
    EXPECT_TRUE(y >= 4.8 && y <= 5.1);
}
