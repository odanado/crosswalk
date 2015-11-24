#ifndef INCLUDE_CROSSWALK_ENDGAME_AI_HPP
#define INCLUDE_CROSSWALK_ENDGAME_AI_HPP

#include "Board.hpp"
#include "EndGameEval.hpp"
#include "Config.hpp"
#include "Utils.hpp"

#include <iostream>
#include <functional>

namespace crosswalk {

class EndGameAI {
private:
    i64 nodeCount;
    i64 leafCount;
    i64 score;
public:
    CellType operator()(const Board &board, CellState color) noexcept {
        nodeCount = 0;
        leafCount = 0;

        CellType result(-1,-1);
        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();
        i64 a=alpha;
        i64 b=beta;
        i64 evalValue  = minValue<i64>();
        i64 maxValue = evalValue;
        bool first=true;

        myColor = color;
        auto cells = board.makeReversibleCells(color);

        moveOrdering(cells, board, color, 8, eval, std::greater<i64>());

        for(const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);

            evalValue = -dfs(nextBoard, switchCellState(color), -b, -a);
            if(a<evalValue && evalValue<beta && !first) a=-dfs(nextBoard, switchCellState(color), -beta, -evalValue);

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

    i64 getNodeCount() const noexcept {
        return nodeCount;
    }

    i64 getLeafCount() const noexcept {
        return leafCount;
    }

    i64 getScore() const noexcept {
        return score;
    }
private:
    EndGameEval eval;
    CellState myColor;
    i64 dfs(const Board &board, CellState color, i64 alpha, i64 beta) noexcept {
        assert(alpha <= beta);
        ++nodeCount;
        if(board.isFinished()) {
            ++leafCount;
            if(color == myColor)
                return eval(board, myColor);
            else
                return -eval(board, myColor);
        }

        auto cells = board.makeReversibleCells(color);
        if(cells.empty()) {
            return -dfs(board, switchCellState(color), -beta, -alpha);
        }

        i64 value;
        i64 a=alpha;
        i64 b=beta;
        bool first=true;

        for(const auto &cell : cells) {
            auto nextBoard(board);
            nextBoard.putStone(color, cell);

            value = -dfs(nextBoard, switchCellState(color), -b, -a);
            if(a<value && value<beta && !first) a=-dfs(nextBoard, switchCellState(color),-beta, -value);
            a=std::max(a, value);
            if(a>=beta)
                return a;
            b=a+1;
            first=false;
        }

        return a;
    }
};

} // end crosswalk

#endif

