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

TEST_F(FeatureTest, getVector2Count) {
    EXPECT_EQ(feature.getVector2Count(3634), 1);
    EXPECT_EQ(feature.getVector2Count(3283), 1);
    EXPECT_EQ(feature.getVector2Count(4127), 1);
    EXPECT_EQ(feature.getVector2Count(3635), 1);
}

TEST_F(FeatureTest, getVector3Count) {
    EXPECT_EQ(feature.getVector3Count(3553), 1);
    EXPECT_EQ(feature.getVector3Count(3526), 1);
    EXPECT_EQ(feature.getVector3Count(3551), 1);
    EXPECT_EQ(feature.getVector3Count(4019), 1);
}
TEST_F(FeatureTest, getVector4Count) {
    EXPECT_EQ(feature.getVector4Count(4039), 1);
    EXPECT_EQ(feature.getVector4Count(4021), 1);
    EXPECT_EQ(feature.getVector4Count(4091), 1);
    EXPECT_EQ(feature.getVector4Count(4253), 1);
}
TEST_F(FeatureTest, getDiag4Count) {
    EXPECT_EQ(feature.getDiag4Count(40), 1);
    EXPECT_EQ(feature.getDiag4Count(52), 1);
    EXPECT_EQ(feature.getDiag4Count(50), 1);
    EXPECT_EQ(feature.getDiag4Count(41), 1);
}
TEST_F(FeatureTest, getDiag5Count) {
    EXPECT_EQ(feature.getDiag5Count(160), 1);
    EXPECT_EQ(feature.getDiag5Count(151), 1);
    EXPECT_EQ(feature.getDiag5Count(158), 1);
    EXPECT_EQ(feature.getDiag5Count(149), 1);
}
TEST_F(FeatureTest, getDiag6Count) {
    EXPECT_EQ(feature.getDiag6Count(448), 1);
    EXPECT_EQ(feature.getDiag6Count(457), 1);
    EXPECT_EQ(feature.getDiag6Count(446), 1);
    EXPECT_EQ(feature.getDiag6Count(473), 1);
}
TEST_F(FeatureTest, getDiag7Count) {
    EXPECT_EQ(feature.getDiag7Count(1456), 1);
    EXPECT_EQ(feature.getDiag7Count(1093), 1);
    EXPECT_EQ(feature.getDiag7Count(1337), 1);
    EXPECT_EQ(feature.getDiag7Count(1094), 1);
}
TEST_F(FeatureTest, getDiag8Count) {
    EXPECT_EQ(feature.getDiag8Count(6479), 1);
    EXPECT_EQ(feature.getDiag8Count(3280), 1);
}
TEST_F(FeatureTest, getEdge2XCount) {
    EXPECT_EQ(feature.getEdge2XCount(29528), 3);
    EXPECT_EQ(feature.getEdge2XCount(32804), 1);
}
TEST_F(FeatureTest, getCorner3x3Count) {
    EXPECT_EQ(feature.getCorner3x3Count(16484), 1);
    EXPECT_EQ(feature.getCorner3x3Count(9871), 1);
    EXPECT_EQ(feature.getCorner3x3Count(16496), 1);
    EXPECT_EQ(feature.getCorner3x3Count(10840), 1);
}
TEST_F(FeatureTest, getCorner2x5Count) {
    EXPECT_EQ(feature.getCorner2x5Count(56498), 2);
    EXPECT_EQ(feature.getCorner2x5Count(57955), 2);
    EXPECT_EQ(feature.getCorner2x5Count(30374), 2);
    EXPECT_EQ(feature.getCorner2x5Count(29644), 2);
}
