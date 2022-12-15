#include "cell.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(CellTest, NeighborsSum) {
    CellMatrix cellMatrixEmpty{{3, 3}};
    CellMatrix cellMatrixFull{{3, 3}, Cell{1}};
    EXPECT_EQ(cellMatrixEmpty.NeighborsSum({1, 1}), 0);
    EXPECT_EQ(cellMatrixFull.NeighborsSum({1, 1}), 8);
}
