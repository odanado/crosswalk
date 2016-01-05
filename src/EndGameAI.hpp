#ifndef INCLUDE_CROSSWALK_ENDGAME_AI_HPP
#define INCLUDE_CROSSWALK_ENDGAME_AI_HPP

#include "Board.hpp"
#include "AI.hpp"
#include "Config.hpp"
#include "TranspositionTable.hpp"

namespace crosswalk {

class EndGameEval {
public:
    i64 operator()(const Board &board, CellState myColor) const noexcept {
        return board.countStone(myColor) - board.countStone(switchCellState(myColor));
    }
};

class FFHSort {
public:
    void operator()(Cells &cells, const Board &board, CellState color) noexcept {
        std::array<i64, 64> order;
        auto nextColor = switchCellState(color);
        for (const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);
            order[cell.toInt()] = nextBoard.getReversibleCount(nextColor);
        }

        std::sort(cells.begin(), cells.end(),
            [order](const CellType &cell1, const CellType &cell2) {
            return order[cell1.toInt()] < order[cell2.toInt()];
        });
    }
};

using EndGameAI = AI<EndGameEval, FFHSort>;

} // end crosswalk

#endif

