#include <array>
#include <functional>
#include <gtest/gtest.h>
#include <raster/conv.hh>
#include <raster/wrap.hh>
#include <raster/map.hh>
#include <raster/util/accumulator.hh>
#include <raster/util/constrain.hh>
#include <raster/util/view2d.hh>

TEST(RasterCoreTest, BasicConvTest) {
    std::array<std::array<int,3>,3> identity = {{
        {0,0,0},
        {0,1,0},
        {0,0,0}
    }};
    std::array<std::array<int,3>,3> mat = {{
        {0,1,2},
        {3,4,5},
        {6,7,8}
    }};
    EXPECT_EQ(raster::conv(identity, mat, 1, 1), 4);
}

TEST(RasterCoreTest, WrapTranslateTest) {
    std::array<std::array<int,3>,3> mat = {{
        {0,1,2},
        {3,4,5},
        {6,7,8}
    }};
    auto wrapped = raster::extend(mat);
    EXPECT_EQ(raster::get(wrapped, -1, -1), 0);
    EXPECT_EQ(raster::get(wrapped, 1, -1), 3);
    EXPECT_EQ(raster::get(wrapped, -1, 1), 1);
    EXPECT_EQ(raster::get(wrapped, 1, 3), 5);
    EXPECT_EQ(raster::get(wrapped, 3, 1), 7);
    EXPECT_EQ(raster::get(wrapped, 3, -1), 6);
    EXPECT_EQ(raster::get(wrapped, 3, 3), 8);
    EXPECT_EQ(raster::get(wrapped, -1, 3), 2);
}

TEST(RasterCoreTest, MapTest) {
    std::array<std::array<int,3>,3> mat = {{
        {0,1,2},
        {3,4,5},
        {6,7,8}
    }};
    auto mapped = raster::mapped([](int x){ return x * 2; }, mat);
    EXPECT_EQ(raster::get(mapped, 0, 0), 0);
    EXPECT_EQ(raster::get(mapped, 0, 1), 2);
    EXPECT_EQ(raster::get(mapped, 0, 2), 4);
    EXPECT_EQ(raster::get(mapped, 1, 0), 6);
    EXPECT_EQ(raster::get(mapped, 1, 1), 8);
    EXPECT_EQ(raster::get(mapped, 1, 2), 10);
    EXPECT_EQ(raster::get(mapped, 2, 0), 12);
    EXPECT_EQ(raster::get(mapped, 2, 1), 14);
    EXPECT_EQ(raster::get(mapped, 2, 2), 16);
}

TEST(RasterCoreTest, ConstrainMatTest) {
    std::array<std::array<int,3>,3> mat = {{
        {0,1,2},
        {3,4,5},
        {6,7,8}
    }};
    auto constrained = raster::constrain(mat);
    raster::set(constrained, 0, 0, 100);
    EXPECT_EQ(raster::get(constrained, 0, 0), 100);
    raster::set(constrained, 89, 13, 33);
    EXPECT_EQ(raster::get(constrained, 0, 0), 100);
    EXPECT_EQ(raster::get(constrained, 0, 1), 1);
    EXPECT_EQ(raster::get(constrained, 0, 2), 2);
    EXPECT_EQ(raster::get(constrained, 1, 0), 3);
    EXPECT_EQ(raster::get(constrained, 1, 1), 4);
    EXPECT_EQ(raster::get(constrained, 1, 2), 5);
    EXPECT_EQ(raster::get(constrained, 2, 0), 6);
    EXPECT_EQ(raster::get(constrained, 2, 1), 7);
    EXPECT_EQ(raster::get(constrained, 2, 2), 8);
}

TEST(RasterCoreTest, AccumulatorTest) {
    std::array<std::array<int,3>,3> mat = {{
        {0,0,0},
        {0,0,0},
        {0,0,0}
    }};
    
    auto acc = raster::accumulator(&mat, 
                                   std::mem_fn(&std::pair<int,int>::first),
                                   std::mem_fn(&std::pair<int,int>::second));

    *acc++ = std::make_pair(1,1);
    *acc++ = std::make_pair(1,1);
    *acc++ = std::make_pair(1,1);
    
    EXPECT_EQ(mat[1][1], 3);
}

TEST(RasterCoreTest, Unflatten) {
    std::array<int,9> flatmat = {0,1,2,3,4,5,6,7,8};
    auto mat = raster::as2d(3, 3, flatmat);
    EXPECT_EQ(raster::get(mat, 0, 0), 0);
    EXPECT_EQ(raster::get(mat, 0, 1), 1);
    EXPECT_EQ(raster::get(mat, 0, 2), 2);
    EXPECT_EQ(raster::get(mat, 1, 0), 3);
    EXPECT_EQ(raster::get(mat, 1, 1), 4);
    EXPECT_EQ(raster::get(mat, 1, 2), 5);
    EXPECT_EQ(raster::get(mat, 2, 0), 6);
    EXPECT_EQ(raster::get(mat, 2, 1), 7);
    EXPECT_EQ(raster::get(mat, 2, 2), 8);
    raster::set(mat, 1, 1, 100);
    EXPECT_EQ(raster::get(mat, 1, 1), 100);
    EXPECT_EQ(flatmat[4], 100);
}

