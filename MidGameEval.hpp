#ifndef INCLUDE_CROSSWALK_MIDGAME_EVAL_HPP
#define INCLUDE_CROSSWALK_MIDGAME_EVAL_HPP

#include <array>
#include "Board.hpp"
#include "Utils.hpp"

namespace crosswalk {
class MidGameEval {
    std::array<i64, 64> table = {{
        100, -40,   5,   0,   0,   5, -40, 100,
        -40, -50, -10, -10, -10, -10, -50, -40,
        5, -10,   0,  -3,  -3,   0, -10,   5,
        0, -10,  -3,  -3,  -3,  -3, -10,   0,
        0, -10,  -3,  -3,  -3,  -3, -10,   0,
        5, -10,   0,  -3,  -3,   0, -10,   5,
        -40, -50, -10, -10, -10, -10, -50, -40,
        100, -40,   5,   0,   0,   5, -40, 100
    }};

    i64 eval(const Board &board, CellState color, u32 k) const noexcept{
        if(board.existStone(color, k >> 3, k & 7))
            return table[k];
        else if(board.existStone(switchCellState(color), k >> 3, k & 7))
            return -table[k];
        else
            return 0;
    }

    i64 add(const Board &board, CellState color, u32 y, u32 x) const noexcept {
        if(board.existStone(color, y, x))
            return table[(y << 3) + x];
        else
            return 0;
    }
    
    i64 corner(const Board &board, CellState color) const noexcept {
        i64 ret = 0;
        if(board.existStone(color, 0, 0)) {
            ret += -add(board, color, 0, 0) * 2;
            ret += -add(board, color, 1, 0) * 2;
            ret += -add(board, color, 1, 1) * 2;
        }

        if(board.existStone(color, 0, 7)) {
            ret += -add(board, color, 0, 6) * 2;
            ret += -add(board, color, 1, 6) * 2;
            ret += -add(board, color, 1, 7) * 2;
        }

        if(board.existStone(color, 7, 0)) {
            ret += -add(board, color, 6, 0) * 2;
            ret += -add(board, color, 6, 1) * 2;
            ret += -add(board, color, 7, 1) * 2;
        }

        if(board.existStone(color, 7, 7)) {
            ret += -add(board, color, 7, 6) * 2;
            ret += -add(board, color, 6, 6) * 2;
            ret += -add(board, color, 6, 7) * 2;
        }

        return ret;
    }
public:
    i64 operator()(const Board &board, CellState color) const noexcept {
        i64 ret = 0;
        for(u32 k = 0; k < 64; k++) {
            ret += eval(board, color, k);
        }

        ret += corner(board, color);
        ret += corner(board, switchCellState(color));

        ret += board.getReversibleCount(color) * 10;
        ret -= board.getReversibleCount(switchCellState(color)) * 10;

        return ret;
    }
};
} // namespace crosswalk

#endif

