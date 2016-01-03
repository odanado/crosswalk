#include "../src/EndGame.hpp"
#include "gtest/gtest.h"

class EndGameEvalTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(EndGameEvalTest, eval) {
    using namespace crosswalk;
    EndGameEval eval;
    auto board = Board(0x0076665a3a7efe80, 0xff8999a5c581017f);

    EXPECT_EQ(eval(board, CellState::WHITE), 2);
    EXPECT_EQ(eval(board, CellState::BLACK), -2);
}
