#include "../src/Feature.hpp"
#include "gtest/gtest.h"
#include "../src/Board.hpp"

class FeatureTest : public ::testing::Test {
 protected:
    virtual void SetUp() {}
};

TEST_F(FeatureTest, getVector2) {
    using namespace crosswalk;
    Feature feature;
    /*
     * x.xxxx.x
     * ........
     * x.oooo.x
     * x.oooo.x
     * x.oooo.x.
     * x.oooo.x
     * ........
     * x.xxxx.x
     */
    feature.update(0xbd008181818100bd, 0x00003c3c3c3c0000);
    EXPECT_EQ(feature.getVector2(0), 4);

    /*
     * ........
     * .xo.xo.x
     * ........
     * ........
     * ........
     * ........
     * x.ox.ox.
     * ........
     */
    feature.update(0x0049000000009200, 0x0024000000002400);
    EXPECT_EQ(feature.getVector2(1261), 2);
    EXPECT_EQ(feature.getVector2(3), 2);

    /*
     * ......x.
     * .x......
     * .o....o.
     * ......x.
     * .x......
     * .o....o.
     * ......x.
     * .x......
     */
    feature.update(0x0240000240000240, 0x0000420000420000);
    EXPECT_EQ(feature.getVector2(1261), 2);
    EXPECT_EQ(feature.getVector2(3), 2);
}

