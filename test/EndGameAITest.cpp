#include "../src/AI.hpp"
#include "../src/EndGame.hpp"
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

    AI<EndGameEval, FFHSort> ai;
    ai.setNormalDFSDepth(6);

    auto cell = ai(board, CellState::BLACK, board.getEmptyCount());

    EXPECT_EQ(ai.getScore(), 62);
    EXPECT_EQ(cell.toString(), "g2");
}


