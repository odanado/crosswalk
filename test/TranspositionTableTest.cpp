#include "../src/TranspositionTable.hpp"
#include "gtest/gtest.h"

class TranspositionTableTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(TranspositionTableTest, init) {
    using namespace crosswalk;
    TranspositionTable cache;
    Board board;
    EXPECT_EQ(cache[NodeState(board, CellState::BLACK)].getAlpha(), minValue<i64>());
    EXPECT_EQ(cache[NodeState(board, CellState::BLACK)].getBeta(), maxValue<i64>());
}
TEST_F(TranspositionTableTest, update) {
    using namespace crosswalk;
    TranspositionTable cache;
    Board board;

    // [-10,10]
    cache.update(NodeState(board, CellState::BLACK), -10, 10);
    EXPECT_EQ(cache[NodeState(board, CellState::BLACK)].getAlpha(), -10);
    EXPECT_EQ(cache[NodeState(board, CellState::BLACK)].getBeta(), 10);

    // [-10,10] and [5,20] = [5,10]
    cache.update(NodeState(board, CellState::BLACK), 5, 20);
    EXPECT_EQ(cache[NodeState(board, CellState::BLACK)].getAlpha(), 5);
    EXPECT_EQ(cache[NodeState(board, CellState::BLACK)].getBeta(), 10);
}
