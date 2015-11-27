#include "../Board.hpp"
#include "../Config.hpp"
#include "gtest/gtest.h"

#include <algorithm>

class BoardTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(BoardTest, Board) {
    using namespace crosswalk;
    Board board;
    EXPECT_EQ(board.existStone(CellState::BLACK, 4, 3), true);
    EXPECT_EQ(board.existStone(CellState::BLACK, 3, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 3, 3), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 4), true);

    EXPECT_EQ(board == board, true);

    board = Board(0x8080c0c48c8080, 0x10783f3b737e79);
    EXPECT_EQ(board.existStone(CellState::WHITE, 0, 0), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 2, 0), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 3, 0), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 0), true);

    EXPECT_EQ(board.existStone(CellState::WHITE, 1, 1), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 2, 1), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 3, 1), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 1), true);

    EXPECT_EQ(board.existStone(CellState::WHITE, 1, 2), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 2), true);

    EXPECT_EQ(board.existStone(CellState::WHITE, 0, 3), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 1, 3), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 3, 3), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 3), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 5, 3), true);

    EXPECT_EQ(board.existStone(CellState::WHITE, 0, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 1, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 2, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 3, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 5, 4), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 6, 4), true);

    EXPECT_EQ(board.existStone(CellState::WHITE, 0, 5), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 1, 5), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 2, 5), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 3, 5), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 4, 5), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 5, 5), true);

    EXPECT_EQ(board.existStone(CellState::WHITE, 0, 6), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 1, 6), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 2, 6), true);
    EXPECT_EQ(board.existStone(CellState::WHITE, 5, 6), true);

    EXPECT_EQ(board.countStone(CellState::WHITE), u64(32));

    EXPECT_EQ(board.existStone(CellState::BLACK, 0, 7), true);

    EXPECT_EQ(board.existStone(CellState::BLACK, 1, 7), true);

    EXPECT_EQ(board.existStone(CellState::BLACK, 2, 2), true);
    EXPECT_EQ(board.existStone(CellState::BLACK, 2, 3), true);
    EXPECT_EQ(board.existStone(CellState::BLACK, 2, 7), true);

    EXPECT_EQ(board.existStone(CellState::BLACK, 3, 2), true);
    EXPECT_EQ(board.existStone(CellState::BLACK, 3, 6), true);
    EXPECT_EQ(board.existStone(CellState::BLACK, 3, 7), true);

    EXPECT_EQ(board.existStone(CellState::BLACK, 4, 6), true);
    EXPECT_EQ(board.existStone(CellState::BLACK, 4, 7), true);

    EXPECT_EQ(board.existStone(CellState::BLACK, 5, 7), true);

    EXPECT_EQ(board.existStone(CellState::BLACK, 6, 7), true);

    EXPECT_EQ(board.countStone(CellState::BLACK), u64(12));
}

TEST_F(BoardTest, putStone) {
    using namespace crosswalk;
    auto board = Board(0x8080c0c48c8080, 0x10783f3b737e79);

    board.putStone(CellState::BLACK, 5, 2);
    EXPECT_EQ(board.getBitBoard(CellState::BLACK), u64(0x0080fcccd4acc080));
    EXPECT_EQ(board.getBitBoard(CellState::WHITE), u64(0x001000332b533e79));

    // 変化しない手
    board.putStone(CellState::BLACK, 7, 7);
    EXPECT_EQ(board.getBitBoard(CellState::BLACK), u64(0x0080fcccd4acc080));
    EXPECT_EQ(board.getBitBoard(CellState::WHITE), u64(0x001000332b533e79));


    /*
     * xxxxxxxx
     * xoooooox
     * xoooooox
     * xoo.ooox
     * xoooooox
     * xoooooox
     * xoooooox
     * xxxxxxxx
     */
    board = Board(0xff818181818181ff, 0x007e7e7e767e7e00);
    board.putStone(CellState::BLACK, 3, 3);
    EXPECT_EQ(board.getBitBoard(CellState::BLACK), u64(0xffc9ab9dff9dabff));
    EXPECT_EQ(board.getBitBoard(CellState::WHITE), u64(0x0036546200625400));


}

TEST_F(BoardTest, makeReversibleCells) {
    using namespace crosswalk;

    auto board = Board(0x8080c0c48c8080, 0x10783f3b737e79);
    auto cells = board.makeReversibleCells(CellState::BLACK);
    std::sort(cells.begin(),cells.end());

    EXPECT_EQ(cells.size(), std::size_t(10));
    EXPECT_EQ(cells[0], CellType(0,1));
    EXPECT_EQ(cells[1], CellType(0,2));
    EXPECT_EQ(cells[2], CellType(1,0));
    EXPECT_EQ(cells[3], CellType(5,0));
    EXPECT_EQ(cells[4], CellType(5,2));
    EXPECT_EQ(cells[5], CellType(6,2));
    EXPECT_EQ(cells[6], CellType(6,3));
    EXPECT_EQ(cells[7], CellType(6,5));
    EXPECT_EQ(cells[8], CellType(6,6));
    EXPECT_EQ(cells[9], CellType(7,3));

    cells = board.makeReversibleCells(CellState::WHITE);
    EXPECT_EQ(cells.size(), std::size_t(0));

    board = Board(0x001834261f004000, 0x4e240b18607e3c3e);
    cells = board.makeReversibleCells(CellState::BLACK);
    std::sort(cells.begin(),cells.end());

    EXPECT_EQ(cells.size(), std::size_t(10));
    EXPECT_EQ(cells[0], CellType(0,6));
    EXPECT_EQ(cells[1], CellType(1,0));
    EXPECT_EQ(cells[2], CellType(1,1));
    EXPECT_EQ(cells[3], CellType(2,7));
    EXPECT_EQ(cells[4], CellType(3,7));
    EXPECT_EQ(cells[5], CellType(4,6));
    EXPECT_EQ(cells[6], CellType(6,0));
    EXPECT_EQ(cells[7], CellType(6,1));
    EXPECT_EQ(cells[8], CellType(6,6));
    EXPECT_EQ(cells[9], CellType(7,5));

}

TEST_F(BoardTest, isFinished) {
    using namespace crosswalk;

    auto board = Board();
    EXPECT_EQ(board.isFinished(), false);

    board = Board(0xffefff5f7f3f1f0f, 0x001000a080c0e0f0);
    EXPECT_EQ(board.isFinished(), true);
}

TEST_F(BoardTest, getReversibleCount) {
    using namespace crosswalk;
    auto board = Board(0x008080C0C48C8080,0x0010783F3B737E79);

    EXPECT_EQ(board.getReversibleCount(CellState::BLACK), 10);
    EXPECT_EQ(board.getReversibleCount(CellState::WHITE),  0);
}

TEST_F(BoardTest, hash) {
    using namespace crosswalk;
    Board board1;
    Board board2 = Board(0xffefff5f7f3f1f0f, 0x001000a080c0e0f0);

    EXPECT_EQ(std::hash<Board>()(board1), std::hash<Board>()(board1));
    EXPECT_NE(std::hash<Board>()(board1), std::hash<Board>()(board2));
}

