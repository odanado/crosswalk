#ifndef INCLUDE_CROSSWALK_TABLEBASE_EVAL_HPP
#define INCLUDE_CROSSWALK_TABLEBASE_EVAL_HPP

#include <array>
#include "Board.hpp"
#include "Utils.hpp"

namespace crosswalk {

class TableBaseEval {
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
public:
    i64 operator()(const Board &board, CellState color) const noexcept {
        i64 ret = 0;
        for(i32 k = 0; k < 64; k++) {
            i32 y = k >> 3;
            i32 x = k & 7;
            if(board.existStone(color, y, x))
                ret += table[k];
            if(board.existStone(switchCellState(color), y, x))
                ret -= table[k];
        }

        return ret;
    }

};

} // namespace crosswalk

#endif

