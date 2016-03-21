#include "../src/Feature.hpp"
#include "gtest/gtest.h"
#include "../src/Board.hpp"

class FeatureTest : public ::testing::Test {
 protected:
    crosswalk::Feature feature;
    virtual void SetUp() {
        /*
         * oxxxxxxx
         * xoxoooxx
         * xxoxoxox
         * xoxoxxox
         * xooxxxox
         * xoxxxoxx
         * xxxxxxox
         * xooooooo
         */
        feature.update(0x01bfddb9b5abc5fe, 0xfe4022464a543a01);
    }
};

TEST_F(FeatureTest, getVector2) {
    EXPECT_EQ(feature.getVector2(3634), 1);
    EXPECT_EQ(feature.getVector2(3283), 1);
    EXPECT_EQ(feature.getVector2(4127), 1);
    EXPECT_EQ(feature.getVector2(3635), 1);
}

TEST_F(FeatureTest, getVector3) {
    EXPECT_EQ(feature.getVector3(3553), 1);
    EXPECT_EQ(feature.getVector3(3526), 1);
    EXPECT_EQ(feature.getVector3(3551), 1);
    EXPECT_EQ(feature.getVector3(4019), 1);
}
TEST_F(FeatureTest, getVector4) {
    EXPECT_EQ(feature.getVector4(4039), 1);
    EXPECT_EQ(feature.getVector4(4021), 1);
    EXPECT_EQ(feature.getVector4(4091), 1);
    EXPECT_EQ(feature.getVector4(4253), 1);
}
TEST_F(FeatureTest, getDiagonal4) {
    EXPECT_EQ(feature.getDiagonal4(40), 1);
    EXPECT_EQ(feature.getDiagonal4(52), 1);
    EXPECT_EQ(feature.getDiagonal4(50), 1);
    EXPECT_EQ(feature.getDiagonal4(41), 1);
}
TEST_F(FeatureTest, getDiagonal5) {
    EXPECT_EQ(feature.getDiagonal5(160), 1);
    EXPECT_EQ(feature.getDiagonal5(151), 1);
    EXPECT_EQ(feature.getDiagonal5(158), 1);
    EXPECT_EQ(feature.getDiagonal5(149), 1);
}
TEST_F(FeatureTest, getDiagonal6) {
    EXPECT_EQ(feature.getDiagonal6(448), 1);
    EXPECT_EQ(feature.getDiagonal6(457), 1);
    EXPECT_EQ(feature.getDiagonal6(446), 1);
    EXPECT_EQ(feature.getDiagonal6(473), 1);
}
TEST_F(FeatureTest, getDiagonal7) {
    EXPECT_EQ(feature.getDiagonal7(1456), 1);
    EXPECT_EQ(feature.getDiagonal7(1093), 1);
    EXPECT_EQ(feature.getDiagonal7(1337), 1);
    EXPECT_EQ(feature.getDiagonal7(1094), 1);
}
TEST_F(FeatureTest, getDiagonal8) {
    EXPECT_EQ(feature.getDiagonal8(6479), 1);
    EXPECT_EQ(feature.getDiagonal8(3280), 1);
}
TEST_F(FeatureTest, getEdge2X) {
    EXPECT_EQ(feature.getEdge2X(29528), 3);
    EXPECT_EQ(feature.getEdge2X(32804), 1);
}
