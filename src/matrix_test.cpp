#include "matrix.h"
#include <gtest/gtest.h>
#include <set>

// Demonstrate some basic assertions.
TEST(MatrixTest, ForEach) {
    int32_t x = 16;
    int32_t y = 13;
    Matrix<uint32_t> mat({x, y});
    uint32_t counter = 0;
    std::set<Coord> setCoord;
    mat.ForEach([&counter, &setCoord](const Coord& c, uint32_t& item) {
        item = counter;
        auto it = setCoord.emplace(c);
        EXPECT_TRUE(it.second);
        ++counter;
    });
    EXPECT_EQ(counter, x * y);
    counter = 0;
    mat.ForEach([&counter](const Coord& c, uint32_t& item) {
        EXPECT_EQ(item, counter);
        ++counter;
    });
}
