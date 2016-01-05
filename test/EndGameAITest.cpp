#include "../src/EndGameAI.hpp"
#include "gtest/gtest.h"

class EndGameAITest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(EndGameAITest, ai) {
    using namespace crosswalk;
    auto board = Board(0x7fbfbfbdbf9c0800, 0x0040404240633428);
    i64 depth = 64 - board.countStone(CellState::BLACK) - board.countStone(CellState::WHITE);

    EndGameAI ai;
    ai.setNormalDFSDepth(6);

    auto cell = ai(board, CellState::BLACK, board.getEmptyCount());

    EXPECT_EQ(ai.getScore(), 62);
    EXPECT_EQ(cell.toString(), "g2");
}

TEST_F(EndGameAITest, eval) {
    using namespace crosswalk;
    EndGameEval eval;
    auto board = Board(0x0076665a3a7efe80, 0xff8999a5c581017f);

    EXPECT_EQ(eval(board, CellState::WHITE), 2);
    EXPECT_EQ(eval(board, CellState::BLACK), -2);
}
