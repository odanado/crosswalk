#ifndef INCLUDE_CROSSWALK_ENDGAME_EVAL_HPP
#define INCLUDE_CROSSWALK_ENDGAME_EVAL_HPP

#include "Board.hpp"
#include "Utils.hpp"

namespace crosswalk {

class EndGameEval {
public:
    i64 operator()(const Board &board, CellState myColor) const noexcept {
        return board.countStone(myColor) - board.countStone(switchCellState(myColor));
    }
};

} // end crosswalk

#endif

