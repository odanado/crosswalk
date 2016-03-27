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
TEST_F(FeatureTest, getDiag4) {
    EXPECT_EQ(feature.getDiag4(40), 1);
    EXPECT_EQ(feature.getDiag4(52), 1);
    EXPECT_EQ(feature.getDiag4(50), 1);
    EXPECT_EQ(feature.getDiag4(41), 1);
}
TEST_F(FeatureTest, getDiag5) {
    EXPECT_EQ(feature.getDiag5(160), 1);
    EXPECT_EQ(feature.getDiag5(151), 1);
    EXPECT_EQ(feature.getDiag5(158), 1);
    EXPECT_EQ(feature.getDiag5(149), 1);
}
TEST_F(FeatureTest, getDiag6) {
    EXPECT_EQ(feature.getDiag6(448), 1);
    EXPECT_EQ(feature.getDiag6(457), 1);
    EXPECT_EQ(feature.getDiag6(446), 1);
    EXPECT_EQ(feature.getDiag6(473), 1);
}
TEST_F(FeatureTest, getDiag7) {
    EXPECT_EQ(feature.getDiag7(1456), 1);
    EXPECT_EQ(feature.getDiag7(1093), 1);
    EXPECT_EQ(feature.getDiag7(1337), 1);
    EXPECT_EQ(feature.getDiag7(1094), 1);
}
TEST_F(FeatureTest, getDiag8) {
    EXPECT_EQ(feature.getDiag8(6479), 1);
    EXPECT_EQ(feature.getDiag8(3280), 1);
}
TEST_F(FeatureTest, getEdge2X) {
    EXPECT_EQ(feature.getEdge2X(29528), 3);
    EXPECT_EQ(feature.getEdge2X(32804), 1);
}
TEST_F(FeatureTest, getCorner3x3) {
    EXPECT_EQ(feature.getCorner3x3(16484), 1);
    EXPECT_EQ(feature.getCorner3x3(9871), 1);
    EXPECT_EQ(feature.getCorner3x3(16496), 1);
    EXPECT_EQ(feature.getCorner3x3(10840), 1);
}
TEST_F(FeatureTest, getCorner2x5) {
    EXPECT_EQ(feature.getCorner2x5(56498), 2);
    EXPECT_EQ(feature.getCorner2x5(57955), 2);
    EXPECT_EQ(feature.getCorner2x5(30374), 2);
    EXPECT_EQ(feature.getCorner2x5(29644), 2);
}
