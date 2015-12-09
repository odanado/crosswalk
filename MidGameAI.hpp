#ifndef INCLUDE_CROSSWALK_MIDGAME_AI_HPP
#define INCLUDE_CROSSWALK_MIDGAME_AI_HPP

#include "Board.hpp"
#include "Utils.hpp"

namespace crosswalk {

template<class Eval>
class MidGameAI {

    Eval eval;
    i64 score;
public:
    CellType operator()(const Board &board, CellState color, i32 depth) noexcept {
        CellType result(-1,-1);
        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();
        i64 a=alpha;
        i64 b=beta;
        i64 evalValue  = minValue<i64>();
        i64 maxValue = evalValue;
        bool first=true;

        auto cells = board.makeReversibleCells(color);
        moveOrdering(cells, board, color, 8, eval, std::greater<i64>());

        for(const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);

            evalValue = -dfs(nextBoard, switchCellState(color), -b, -a, depth - 1);
            if(a<evalValue && evalValue<beta && !first) a=-dfs(nextBoard, switchCellState(color), -beta, -evalValue, depth - 1);

            a=std::max(a,evalValue);
            b=a+1;
            first=false;

            if(maxValue<a) {
                maxValue=a;
                result=cell;
            }
        }

        score = maxValue;

        return result;
    }

private:
    i64 dfs(const Board &board, CellState color, i64 alpha, i64 beta, i32 depth) noexcept {
        assert(alpha <= beta);
        if(depth == 0 || board.isFinished()) {
            return eval(board, color);
        }
        auto cells = board.makeReversibleCells(color);
        if(cells.empty()) {
            return -dfs(board, switchCellState(color), -beta, -alpha, depth);
        }

        i64 value;
        i64 a=alpha;
        i64 b=beta;
        bool first=true;

        for(const auto &cell : cells) {
            auto nextBoard(board);
            nextBoard.putStone(color, cell);

            value = -dfs(nextBoard, switchCellState(color), -b, -a, depth - 1);
            if(a<value && value<beta && !first) a=-dfs(nextBoard, switchCellState(color),-beta, -value, depth - 1);
            a=std::max(a, value);
            if(a>=beta) {
                // cache.update(board, a, maxValue<i64>());
                return a;
            }
            b=a+1;
            first=false;
        }

        return a;
    }

};

} // namespace crosswalk

#endif

