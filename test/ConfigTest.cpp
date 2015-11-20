#include "../Config.hpp"
#include "gtest/gtest.h"

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

