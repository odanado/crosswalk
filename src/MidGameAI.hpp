#ifndef INCLUDE_CROSSWALK_MIDGAME_AI_HPP
#define INCLUDE_CROSSWALK_MIDGAME_AI_HPP

#include "Board.hpp"
#include "AI.hpp"
#include "Utils.hpp"

namespace crosswalk {

template<class Eval>
class MoveOrdering {
public:
    void operator()(Cells &cells, const Board &board, CellState color, const Eval &eval) noexcept {
        moveOrdering(cells, board, color, 5, eval, std::greater<i64>());
    }
};

template<class Eval>
using MidGameAI = AI<Eval, MoveOrdering<Eval>>;


} // namespace crosswalk

#endif

