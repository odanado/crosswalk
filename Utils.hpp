#ifndef INCLUDE_CROSSWALK_UTILS_HPP
#define INCLUDE_CROSSWALK_UTILS_HPP

#include "Config.hpp"
#include "Board.hpp"
#include <algorithm>
#include <iostream>

namespace crosswalk {

    CellState switchCellState(CellState color) noexcept {
        if(color == CellState::BLACK)
            return CellState::WHITE;
        else
            return CellState::BLACK;
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
