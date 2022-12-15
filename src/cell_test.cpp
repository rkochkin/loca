#include "cell.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    CellMatrix cellMatrix{{3,3}};
    EXPECT_EQ(cellMatrix.NeighborsSum({1,1}),0);
}
