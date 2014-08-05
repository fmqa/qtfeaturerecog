#include <array>
#include <functional>
#include <iterator>
#include <algorithm>
#include <vector>
#include <gtest/gtest.h>
#include <util/circles.hh>
#include <raster/util/flat_accumulator.hh>

TEST(CirclesTest, CircleBoundsTest) {
    circles::parametric_circle<int> circ{0, 0, 5};
    EXPECT_TRUE(circles::within(0, 0, circ));
    EXPECT_TRUE(circles::within(0, 4, circ));
    EXPECT_TRUE(circles::within(0, -4, circ));
    EXPECT_TRUE(circles::within(-3, 0, circ));
    EXPECT_TRUE(circles::within(3, 0, circ));
}

TEST(CirclesTest, CircleTrigPlotTest) {
    std::array<std::pair<double,double>,360> points;
    circles::trigcircle(1.0, circles::unit.begin(), circles::unit.end(), points.begin());
    for (int i = 0; i < 360; ++i) {
        EXPECT_EQ(points[i].first, std::cos(circles::unit[i]));
        EXPECT_EQ(points[i].second, std::sin(circles::unit[i]));
    }
}

TEST(CirclesTest, CircleHoughTest) {
    std::array<std::pair<double,double>,360> points;
    std::vector<int> acc(100 * 100, 0);
    circles::trigcircle(5, circles::unit.begin(), circles::unit.end(), points.begin(), 6, 6);
    circles::hough(5, points.begin(), points.end(), 
                   raster::as_constrained_flat_acc(100, 100, acc),
                   circles::trig(circles::unit.begin(), circles::unit.end()),
                   std::mem_fn(&std::pair<double,double>::first),
                   std::mem_fn(&std::pair<double,double>::second));
    auto m = std::max_element(acc.begin(), acc.end());
    auto index = m - acc.begin();
    auto y = index / 100;
    auto x = index % 100;
    EXPECT_TRUE(y >= 5 && y <= 7);
    EXPECT_TRUE(x >= 5 && x <= 7);
}

