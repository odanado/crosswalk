#include "../src/Utils.hpp"
#include "gtest/gtest.h"

class UtilsTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(UtilsTest, switchCellState) {
    using namespace crosswalk;

    EXPECT_EQ(CellState::BLACK, switchCellState(CellState::WHITE));
    EXPECT_EQ(CellState::WHITE, switchCellState(CellState::BLACK));
}

TEST_F(UtilsTest, negaAlpha) {
    using namespace crosswalk;
    auto eval = [](Board board, CellState myColor) {
        return board.countStone(myColor) - board.countStone(switchCellState(myColor));
    };
    Board board;
    i64 depth;

    board = Board(0x0076464a32305888, 0xff89b9b5cdcf2534);
    depth = 64 + 10 - board.countStone(CellState::BLACK) - board.countStone(CellState::WHITE);
    EXPECT_EQ(negaAlpha(board, CellState::WHITE, CellState::WHITE, eval, minValue<i64>(), maxValue<i64>(), depth), 2);

    board = Board(0x0286ccb8f8e0840c, 0x18383347071f3930);
    depth = 64 + 10 - board.countStone(CellState::BLACK) - board.countStone(CellState::WHITE);
    EXPECT_EQ(negaAlpha(board, CellState::WHITE, CellState::WHITE, eval, minValue<i64>(), maxValue<i64>(), depth), 2);

    board = Board(0x3eb4e3af949d9820, 0x00081c506b62240e);
    depth = 64 + 10 - board.countStone(CellState::BLACK) - board.countStone(CellState::WHITE);
    EXPECT_EQ(negaAlpha(board, CellState::WHITE, CellState::WHITE, eval, minValue<i64>(), maxValue<i64>(), depth), 0);

    board = Board(0x7fbfbfbdbf9c0800, 0x0040404240633428);
    depth = 64 + 10 - board.countStone(CellState::BLACK) - board.countStone(CellState::WHITE);
    EXPECT_EQ(negaAlpha(board, CellState::BLACK, CellState::BLACK, eval, minValue<i64>(), maxValue<i64>(), depth), 62);
}

