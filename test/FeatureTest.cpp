#include "../src/Feature.hpp"
#include "gtest/gtest.h"
#include "../src/Board.hpp"

class FeatureTest : public ::testing::Test {
 protected:
    crosswalk::Feature feature;
    virtual void SetUp() {}
};

TEST_F(FeatureTest, getVector2) {
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

TEST_F(FeatureTest, getVector3) {
    /*
     * xx.xx.xx
     * xx.xx.xx
     * ........
     * xx.oo.xx
     * xx.oo.xx
     * ........
     * xx.xx.xx
     * xx.xx.xx
     */
    feature.update(0xdbdb00c3c300dbdb, 0x0000001818000000);
    EXPECT_EQ(feature.getVector3(0), 4);

    /*
     * ........
     * ........
     * .xo.xo.x
     * ........
     * ........
     * x.ox.ox.
     * ........
     * ........
     */
    feature.update(0x0000490000920000, 0x0000240000240000);
    EXPECT_EQ(feature.getVector3(1261), 2);
    EXPECT_EQ(feature.getVector3(2 * 9 + 2 * 243), 2);

    /*
     * .....x..
     * ..x.....
     * ..o..o..
     * .....x..
     * ..x.....
     * ..o..o..
     * .....x..
     * ..x.....
     */
    feature.update(0x0420000420000420, 0x0000240000240000);
    EXPECT_EQ(feature.getVector3(1261), 2);
    EXPECT_EQ(feature.getVector3(2 * 9 + 2 * 243), 2);
}
TEST_F(FeatureTest, getVector4) {
    /*
     * xxx..xxx
     * xxx..xxx
     * xxx..xxx
     * ........
     * ........
     * xxx..xxx
     * xxx..xxx
     * xxx..xxx
     */
    feature.update(0xe7e7e70000e7e7e7, 0x0000000000000000);
    EXPECT_EQ(feature.getVector4(0), 4);

    /*
     * ........
     * ........
     * ........
     * .xo.xo.x
     * x.ox.ox.
     * ........
     * ........
     * ........
     */
    feature.update(0x0000004992000000, 0x0000002424000000);
    EXPECT_EQ(feature.getVector4(1261), 2);
    EXPECT_EQ(feature.getVector4(27), 2);

    /*
     * ....x...
     * ...x....
     * ...oo...
     * ....x...
     * ...x....
     * ...oo...
     * ....x...
     * ...x....
     */
    feature.update(0x0810000810000810, 0x0000180000180000);
    EXPECT_EQ(feature.getVector4(1261), 2);
    EXPECT_EQ(feature.getVector4(27), 2);
}
TEST_F(FeatureTest, getDiagonal4) {
    /*
     * xxx..xxx
     * xx.oo.xx
     * x.oooo.x
     * .oooooo.
     * .oooooo.
     * x.oooo.x
     * xx.oo.xx
     * xxx..xxx
     */
    feature.update(0xe7c381000081c3e7, 0x00183c7e7e3c1800);
    EXPECT_EQ(feature.getDiagonal4(0), 4);

    /*
     * ........
     * ..x..x..
     * .o....o.
     * x......x
     * ........
     * ........
     * ..x..o..
     * ...xo...
     */
    feature.update(0x0804000081002400, 0x1020000000420000);
    EXPECT_EQ(feature.getDiagonal4(16), 2);
    EXPECT_EQ(feature.getDiagonal4(4), 1);
    EXPECT_EQ(feature.getDiagonal4(8), 1);
}
