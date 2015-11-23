#ifndef INCLUDE_CROSSWALK_UTILS_HPP
#define INCLUDE_CROSSWALK_UTILS_HPP

#include "Config.hpp"
#include "Board.hpp"
#include <algorithm>
#include <iostream>
#include <array>

namespace crosswalk {

    CellState switchCellState(CellState color) noexcept {
        if(color == CellState::BLACK)
            return CellState::WHITE;
        else
            return CellState::BLACK;
    }

    template<class Eval>
    void moveOrdering(Cells &cells, const Board &board, CellState color, const Eval &eval, i64 depth) noexcept {
        std::array<i64, 64> order;

        for(const auto &cell : cells) {
            auto nextBoard(board);
            nextBoard.putStone(color,cell);
            order[cell.toInt()] = -negaAlpha(nextBoard, switchCellState(color), color, eval, minValue<i64>(), maxValue<i64>(), depth);
        }

        std::sort(cells.begin(), cells.end(),
                [order](const CellType &cell1, const CellType &cell2) {
                return order[cell1.toInt()] > order[cell2.toInt()];
                });

    }

    template<class Eval>
    i64 negaAlpha(const Board &board, CellState color, CellState myColor, const Eval &eval, i64 alpha, i64 beta, i64 depth) {
        assert(alpha <= beta);
        if(depth == 0) {
            if(color == myColor)
                return eval(board, myColor);
            else
                return -eval(board, myColor);
        }

        auto cells = board.makeReversibleCells(color);
        if(cells.empty()) {
            return -negaAlpha(board, switchCellState(color), myColor, eval, -beta, -alpha, depth-1);
        }

        auto nextColor = switchCellState(color);
        for(const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);
            alpha = std::max(alpha, -negaAlpha(nextBoard, nextColor, myColor, eval, -beta, -alpha, depth-1));
            
            if(alpha >= beta)
                return beta;
        }

        return alpha;
    }

} // end crosswalk

#endif

