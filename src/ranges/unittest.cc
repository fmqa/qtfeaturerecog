/**
 * 
 * Interval abstraction unit test
 * 
 * g++ -Wall -std=c++11 -I/home/fadimk/Anwendungen/gtest/include -L/home/fadimk/Anwendungen/gtest/lib64 main.cc -lgtest -lgtest_main 
 * 
 * 
 * 
 */

#include <gtest/gtest.h>
#include "interval.h"

TEST(IntervalIterator, Defaults) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(0, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, IncrementIntRange) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(0, *a);
    ++a;
    
    EXPECT_EQ(1, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, IncrementIntRangeTwice) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(0, *a);
    ++a;
    EXPECT_EQ(1, *a);
    ++a;
    
    EXPECT_EQ(2, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, PostIncrementIntRange) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a++;
    
    EXPECT_EQ(1, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, PostIncrementIntRangeTwice) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a++;
    
    EXPECT_EQ(1, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, IncrementIntRangeWithStep) {
    ranges::interval<int> i(0, 10, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    ++a;
    
    EXPECT_EQ(2, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, PostIncrementIntRangeWithStep) {
    ranges::interval<int> i(0, 10, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a++;
    
    EXPECT_EQ(2, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, InitNonZeroOriginRange) {
    ranges::interval<int> i(5, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(5, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, IncrNonZeroOriginRange) {
    ranges::interval<int> i(5, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    ++a;
    
    EXPECT_EQ(6, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, DecrementIntRange) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    ++a;
    --a;
    
    EXPECT_EQ(0, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, DecrementIntRangeTwice) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(0, *a);
    ++a;
    EXPECT_EQ(1, *a);
    ++a;
    EXPECT_EQ(2, *a);
    --a;
    EXPECT_EQ(1, *a);
    --a;
    
    EXPECT_EQ(0, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, DecrementIntRangeWithStep) {
    ranges::interval<int> i(0, 10, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(0, *a);
    ++a;
    EXPECT_EQ(2, *a);
    ++a;
    EXPECT_EQ(4, *a);
    ++a;
    EXPECT_EQ(6, *a);
    --a;
    EXPECT_EQ(4, *a);
    --a;
    
    EXPECT_EQ(2, *a);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, EndOnDecUnderflow) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    --a;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnDecUnderflowNonZeroOrigin) {
    ranges::interval<int> i(5, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    --a;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnSubUnderflow) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a -= 1;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnSubUnderflowNonZeroOrigin) {
    ranges::interval<int> i(5, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    --a;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnSubUnderflowFactor) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a -= 100;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnSubUnderflowFactorNonZeroOrigin) {
    ranges::interval<int> i(5, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a -= 100;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnOverflow) {
    ranges::interval<int> i(0, 2, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_EQ(0, *a);
    ++a;
    EXPECT_EQ(1, *a);
    ++a;
    EXPECT_EQ(2, *a);
    ++a;
    
    EXPECT_EQ(0, *a);
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnAddOverflowFactor) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a += 100;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, EndOnAddUnderflowFactorNonZeroOrigin) {
    ranges::interval<int> i(5, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    a += 100;
    
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, InfixAddDerefTest) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(*(a + 1) == 1);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, InfixAddDerefTestWithStep) {
    ranges::interval<int> i(2, 8, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(*(a + 2) == 6);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, InfixAddDerefTest2) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(*(a + 2) == 2);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, SubscriptTest) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(0 == a[0]);
    EXPECT_TRUE(1 == a[1]);
    EXPECT_TRUE(2 == a[2]);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, NegativeSubscriptTest) {
    ranges::interval<int> i(0, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    ++a;
    EXPECT_EQ(1, *a);
    
    EXPECT_TRUE(0 == a[-1]);
    EXPECT_EQ(0, *b);
}


TEST(IntervalIterator, SubscriptTestWithStep) {
    ranges::interval<int> i(2, 8, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(2 == a[0]);
    EXPECT_TRUE(4 == a[1]);
    EXPECT_TRUE(6 == a[2]);
    EXPECT_EQ(0, *b);
}

TEST(IntervalIterator, UnsignedUnderflowTest) {
    ranges::interval<unsigned char> i(0, 10, 1);
    ranges::interval<unsigned char>::iterator a = i.begin(), b = i.end();
    
    --a;
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, UnsignedOverflowTest) {
    ranges::interval<unsigned char> i(0, 255, 1);
    ranges::interval<unsigned char>::iterator a = i.begin(), b = i.end();
    
    a += 255;
    EXPECT_EQ(255, *a);
    
    ++a;
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, BoolRange) {
    ranges::interval<bool> i(0, 1, 1);
    ranges::interval<bool>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(false == *a);
    EXPECT_TRUE(a != b);
    ++a;
    EXPECT_TRUE(true == *a);
    EXPECT_TRUE(a != b);
    ++a;
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, BoolRangeDec) {
    ranges::interval<bool> i(0, 1, 1);
    ranges::interval<bool>::iterator a = i.begin(), b = i.end();
    
    EXPECT_TRUE(false == *a);
    EXPECT_TRUE(a != b);
    ++a;
    EXPECT_TRUE(true == *a);
    EXPECT_TRUE(a != b);
    --a;
    EXPECT_TRUE(false == *a);
    EXPECT_TRUE(a != b);
}

TEST(IntervalIterator, SignedOverflow) {
    ranges::interval<char> i(0, 127, 1);
    ranges::interval<char>::iterator a = i.begin(), b = i.end();
    
    a += 127;
    EXPECT_EQ(127, *a);
    EXPECT_TRUE(a != b);
    ++a;
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, SignedUnderflow) {
    ranges::interval<char> i(0, 127, 1);
    ranges::interval<char>::iterator a = i.begin(), b = i.end();
    
    --a;
    EXPECT_TRUE(a == b);
}

TEST(IntervalIterator, Distance) {
    ranges::interval<char> i(0, 127, 1);
    ranges::interval<char>::iterator a = i.begin(), b = i.end();
    
    ++a;
    ++a;
    ++a;
    EXPECT_EQ(3, *a);
    
    ranges::interval<char>::iterator c = i.begin();
    ++c;
    EXPECT_EQ(1, *c);
    
    EXPECT_EQ(2, a - c);
    EXPECT_EQ(-2, c - a);
}

TEST(IntervalIterator, StdDistance) {
    ranges::interval<char> i(0, 127, 1);
    ranges::interval<char>::iterator a = i.begin(), b = i.end();
    
    ++a;
    ++a;
    ++a;
    EXPECT_EQ(3, *a);
    
    ranges::interval<char>::iterator c = i.begin();
    ++c;
    
    EXPECT_EQ(-2, std::distance(a, c));
    EXPECT_EQ(2, std::distance(c, a));
}

TEST(IntervalIterator, DistanceWithStep) {
    ranges::interval<int> i(2, 8, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    ++a;
    ++a;
    ++a;
    EXPECT_EQ(8, *a);
    
    ranges::interval<int>::iterator c = i.begin();
    ++c;
    EXPECT_EQ(4, *c);
    
    EXPECT_EQ(2, a - c);
    EXPECT_EQ(-2, c - a);
}

TEST(IntervalIterator, StdDistanceWithStep) {
    ranges::interval<int> i(2, 8, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    
    ++a;
    ++a;
    ++a;
    EXPECT_EQ(8, *a);
    
    ranges::interval<int>::iterator c = i.begin();
    ++c;
    EXPECT_EQ(4, *c);
    
    EXPECT_EQ(2, std::distance(c, a));
    EXPECT_EQ(-2, std::distance(a, c));
}

TEST(IntervalIterator, ForLoopIteration) {
    ranges::interval<int> i(1, 10, 1);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    int count = 0;
    
    for (; a != b; ++a) {
        ++count;
    }
    
    EXPECT_EQ(10, count);
}

TEST(IntervalIterator, ForLoopIterationWithStep) {
    ranges::interval<int> i(2, 8, 2);
    ranges::interval<int>::iterator a = i.begin(), b = i.end();
    int count = 0;
    
    for (; a != b; ++a) {
        ++count;
    }
    
    EXPECT_EQ(4, count);
}
