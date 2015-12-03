#include "../Index.hpp"
#include "gtest/gtest.h"

class IndexTest : public ::testing::Test {
protected:
    crosswalk::Index index;
    virtual void SetUp() {
    }
};

TEST_F(IndexTest, horizontal) {
    index.update(0x0076464a32305888,0xff89b9b5cdcf2534);
    
    EXPECT_EQ(index.getHorizontal(0), 2880);
    EXPECT_EQ(index.getHorizontal(1), 1343);
    EXPECT_EQ(index.getHorizontal(2), 6236);
    EXPECT_EQ(index.getHorizontal(3), 6233);
    EXPECT_EQ(index.getHorizontal(4), 5801);
    EXPECT_EQ(index.getHorizontal(5), 5819);
    EXPECT_EQ(index.getHorizontal(6), 5495);
    EXPECT_EQ(index.getHorizontal(7), 6560);

    index.update(0x0286ccb8f8e0840c, 0x18383347071f3930);
    EXPECT_EQ(index.getHorizontal(0), 684);
    EXPECT_EQ(index.getHorizontal(1), 2900);
    EXPECT_EQ(index.getHorizontal(2), 3401);
    EXPECT_EQ(index.getHorizontal(3), 3293);
    EXPECT_EQ(index.getHorizontal(4), 4022);
    EXPECT_EQ(index.getHorizontal(5), 3608);
    EXPECT_EQ(index.getHorizontal(6), 2901);
    EXPECT_EQ(index.getHorizontal(7), 219);

    index.update(0x3eb4e3af949d9820, 0x00081c506b62240e);
    EXPECT_EQ(index.getHorizontal(0), 321);
    EXPECT_EQ(index.getHorizontal(1), 2799);
    EXPECT_EQ(index.getHorizontal(2), 4255);
    EXPECT_EQ(index.getHorizontal(3), 4283);
    EXPECT_EQ(index.getHorizontal(4), 4090);
    EXPECT_EQ(index.getHorizontal(5), 3397);
    EXPECT_EQ(index.getHorizontal(6), 2574);
    EXPECT_EQ(index.getHorizontal(7), 363);
}

TEST_F(IndexTest, vertical) {
    index.update(0x0076464a32305888,0xff89b9b5cdcf2534);
    EXPECT_EQ(index.getVertical(0), 6558);
    EXPECT_EQ(index.getVertical(1), 5472);
    EXPECT_EQ(index.getVertical(2), 5588);
    EXPECT_EQ(index.getVertical(3), 6475);
    EXPECT_EQ(index.getVertical(4), 5792);
    EXPECT_EQ(index.getVertical(5), 5795);
    EXPECT_EQ(index.getVertical(6), 5502);
    EXPECT_EQ(index.getVertical(7), 6553);

    index.update(0x0286ccb8f8e0840c, 0x18383347071f3930);
    EXPECT_EQ(index.getVertical(0), 726);
    EXPECT_EQ(index.getVertical(1), 3636);
    EXPECT_EQ(index.getVertical(2), 1210);
    EXPECT_EQ(index.getVertical(3), 6208);
    EXPECT_EQ(index.getVertical(4), 6452);
    EXPECT_EQ(index.getVertical(5), 2069);
    EXPECT_EQ(index.getVertical(6), 441);
    EXPECT_EQ(index.getVertical(7), 1092);

    index.update(0x3eb4e3af949d9820, 0x00081c506b62240e);
    EXPECT_EQ(index.getVertical(0), 387);
    EXPECT_EQ(index.getVertical(1), 2585);
    EXPECT_EQ(index.getVertical(2), 3527);
    EXPECT_EQ(index.getVertical(3), 4280);
    EXPECT_EQ(index.getVertical(4), 3603);
    EXPECT_EQ(index.getVertical(5), 3319);
    EXPECT_EQ(index.getVertical(6), 477);
    EXPECT_EQ(index.getVertical(7), 1092);
}

TEST_F(IndexTest, diagLeft) {
    index.update(0x0076464a32305888,0xff89b9b5cdcf2534);
    EXPECT_EQ(index.getDiagLeft(0), 79);
    EXPECT_EQ(index.getDiagLeft(1), 212);
    EXPECT_EQ(index.getDiagLeft(2), 644);
    EXPECT_EQ(index.getDiagLeft(3), 1932);
    EXPECT_EQ(index.getDiagLeft(4), 5467);
    EXPECT_EQ(index.getDiagLeft(5), 1932);
    EXPECT_EQ(index.getDiagLeft(6), 728);
    EXPECT_EQ(index.getDiagLeft(7), 212);
    EXPECT_EQ(index.getDiagLeft(8), 68);

    index.update(0x0286ccb8f8e0840c, 0x18383347071f3930);
    EXPECT_EQ(index.getDiagLeft(0), 76);
    EXPECT_EQ(index.getDiagLeft(1), 242);
    EXPECT_EQ(index.getDiagLeft(2), 728);
    EXPECT_EQ(index.getDiagLeft(3), 699);
    EXPECT_EQ(index.getDiagLeft(4), 1089);
    EXPECT_EQ(index.getDiagLeft(5), 1093);
    EXPECT_EQ(index.getDiagLeft(6), 229);
    EXPECT_EQ(index.getDiagLeft(7), 241);
    EXPECT_EQ(index.getDiagLeft(8), 76);

    index.update(0x3eb4e3af949d9820, 0x00081c506b62240e);
    EXPECT_EQ(index.getDiagLeft(0), 80);
    EXPECT_EQ(index.getDiagLeft(1), 147);
    EXPECT_EQ(index.getDiagLeft(2), 364);
    EXPECT_EQ(index.getDiagLeft(3), 393);
    EXPECT_EQ(index.getDiagLeft(4), 612);
    EXPECT_EQ(index.getDiagLeft(5), 1213);
    EXPECT_EQ(index.getDiagLeft(6), 475);
    EXPECT_EQ(index.getDiagLeft(7), 124);
    EXPECT_EQ(index.getDiagLeft(8), 40);
}

TEST_F(IndexTest, diagRight) {
    index.update(0x0076464a32305888,0xff89b9b5cdcf2534);
    EXPECT_EQ(index.getDiagRight(0), 80);
    EXPECT_EQ(index.getDiagRight(1), 229);
    EXPECT_EQ(index.getDiagRight(2), 608);
    EXPECT_EQ(index.getDiagRight(3), 1914);
    EXPECT_EQ(index.getDiagRight(4), 5823);
    EXPECT_EQ(index.getDiagRight(5), 1916);
    EXPECT_EQ(index.getDiagRight(6), 644);
    EXPECT_EQ(index.getDiagRight(7), 230);
    EXPECT_EQ(index.getDiagRight(8), 68);

    index.update(0x0286ccb8f8e0840c, 0x18383347071f3930);
    EXPECT_EQ(index.getDiagRight(0), 44);
    EXPECT_EQ(index.getDiagRight(1), 124);
    EXPECT_EQ(index.getDiagRight(2), 457);
    EXPECT_EQ(index.getDiagRight(3), 1101);
    EXPECT_EQ(index.getDiagRight(4), 612);
    EXPECT_EQ(index.getDiagRight(5), 701);
    EXPECT_EQ(index.getDiagRight(6), 215);
    EXPECT_EQ(index.getDiagRight(7), 233);
    EXPECT_EQ(index.getDiagRight(8), 71);

    index.update(0x3eb4e3af949d9820, 0x00081c506b62240e);
    EXPECT_EQ(index.getDiagRight(0), 51);
    EXPECT_EQ(index.getDiagRight(1), 158);
    EXPECT_EQ(index.getDiagRight(2), 473);
    EXPECT_EQ(index.getDiagRight(3), 1097);
    EXPECT_EQ(index.getDiagRight(4), 468);
    EXPECT_EQ(index.getDiagRight(5), 447);
    EXPECT_EQ(index.getDiagRight(6), 394);
    EXPECT_EQ(index.getDiagRight(7), 158);
    EXPECT_EQ(index.getDiagRight(8), 40);
}
