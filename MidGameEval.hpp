#ifndef INCLUDE_CROSSWALK_MIDGAME_EVAL_HPP
#define INCLUDE_CROSSWALK_MIDGAME_EVAL_HPP

#include <array>
#include "Board.hpp"
#include "Utils.hpp"
#include <iostream>

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

    i64 eval(const Board &board, CellState color, u32 k) const {
        if(board.existStone(color, k >> 3, k & 7))
            return table[k];
        else if(board.existStone(switchCellState(color), k >> 3, k & 7))
            return -table[k];
        else
            return 0;
    }

    i64 add(const Board &board, CellState color, u32 y, u32 x) const  {
        if(board.existStone(color, y, x))
            return table[(y << 3) + x];
        else
            return 0;
    }
    
    i64 corner(const Board &board, CellState color) const  {
        i64 ret = 0;
        if(board.existStone(color, 0, 0)) {
            ret += -add(board, color, 0, 1) * 2;
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

    i64 stable(const Board &board, CellState color) const {
        i64 ret = 0;
        i32 y, x;

        if (board.existStone(color, 0, 0)) {
            y = 1;
            while (y < 8 && board.existStone(color, y, 0)) {
                y++;
            }
            ret += y;

            x = 1;
            while (x < 8 && board.existStone(color, 0, x)) {
                x++;
            }
            ret += x;
            ret--;
        }

        if (board.existStone(color, 0, 7)) {
            y = 1;
            while (y < 8 && board.existStone(color, y, 7)) {
                y++;
            }
            ret += y;
            
            x = 6;
            while (0 <= x && board.existStone(color, 0, x)) {
                x--;
            }
            if (x != -1) {
                ret += 7 - x;
                ret--;
            }
        }

        if (board.existStone(color, 7, 0)) {
            y = 6;
            while (0 <= y && board.existStone(color, y, 0)) {
                y--;
            }
            if (y != -1) {
                ret += 7 - y;
            }
            
            x = 1;
            while (x < 8 && board.existStone(color, 7, x)) {
                x++;
            }
            ret += x;
            ret--;
        }

        if (board.existStone(color, 7, 7)) {
            y = 6;
            while (0 <= y && board.existStone(color, y, 7)) {
                y--;
            }
            if (y != -1) {
                ret += 7 - y;
            }

            x = 6;
            while (0 <= x && board.existStone(color, 7, x)) {
                x--;
            }
            if (x != -1) {
                ret += 7 - x;
                ret--;
            }
            if (y == -1 && x == -1)
                ret++;
        }

        return ret;

    }

    i64 mountain(const Board &board, CellState color) const {
        i64 ret = 0;
        bool f;
        
        f = board.empty(0, 0) && board.empty(0, 7);
        for (u32 x = 1; x <= 6; x++) {
            f &= board.existStone(color, 0, x);
        }
        ret += f;

        f = board.existStone(color, 7, 0) && board.existStone(color, 7, 7);
        for (u32 x = 1; x <= 6; x++) {
            f &= board.existStone(color, 7, x);
        }
        ret += f;

        f = board.existStone(color, 0, 7) && board.existStone(color, 7, 7);
        for (u32 y = 1; y <= 6; y++) {
            //f &= board.existStone()
        }

        return ret;
    }
public:
    i64 operator()(const Board &board, CellState color) const  {
        if (board.getBitBoard(color) == 0) {
            return minValue<i64>();
        }
        if (board.getBitBoard(switchCellState(color)) == 0) {
            return maxValue<i64>();
        }
        i64 ret = 0;
        for(u32 k = 0; k < 64; k++) {
            ret += eval(board, color, k);
        }

        ret += corner(board, color);
        ret -= corner(board, switchCellState(color));

        ret += board.getReversibleCount(color) * 10;
        ret -= board.getReversibleCount(switchCellState(color)) * 10;

        ret += stable(board, color) * 10;
        ret -= stable(board, switchCellState(color)) * 10;

        return ret;
    }
};
} // namespace crosswalk

#endif

