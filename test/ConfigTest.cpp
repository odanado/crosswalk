#include "../Config.hpp"
#include "gtest/gtest.h"

#include <algorithm>

class ConfigTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(ConfigTest, ConstantValueTest) {
    using namespace crosswalk;
    EXPECT_EQ(maxValue<i64>(), -minValue<i64>());
    EXPECT_EQ(maxValue<u64>(), -minValue<u64>());
}

TEST_F(ConfigTest, CellTypeTest) {
    using namespace crosswalk;
    EXPECT_EQ(0 , CellType(0,0).toInt());
    EXPECT_EQ(10, CellType(1,2).toInt());
    EXPECT_EQ(30, CellType(3,6).toInt());
    EXPECT_EQ(55, CellType(6,7).toInt());
    EXPECT_EQ(63, CellType(7,7).toInt());

    Cells cells;
    for(u32 i=0;i<=1;i++)
        for(u32 j=0;j<=1;j++)
            cells.emplace_back(i,j);

    std::sort(cells.begin(), cells.end());

    EXPECT_EQ(cells[0], CellType(0,0));
    EXPECT_EQ(cells[1], CellType(0,1));
    EXPECT_EQ(cells[2], CellType(1,0));
    EXPECT_EQ(cells[3], CellType(1,1));

    EXPECT_EQ(CellType("g1"), CellType(0,6));
    EXPECT_EQ(CellType(4,5), CellType("f5"));
    EXPECT_EQ(CellType(7,4).toString(), "e8");

}

