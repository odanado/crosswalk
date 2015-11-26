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
    i64 moveOrderingHeight; // move orderingを行う高さの下限
    i64 ffhHeight; // 速さ優先探索を行う高さの下限
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

    i64 getMoveOrderingHeight() const noexcept {
        return moveOrderingHeight;
    }

    void setMoveOrderingHeight(i64 moveOrderingHeight) noexcept {
        this->moveOrderingHeight = moveOrderingHeight;
    }

    i64 getFFHHeight() const noexcept {
        return ffhHeight;
    }
    void setFFHHeight(i64 ffhHeight) noexcept {
        this->ffhHeight = ffhHeight;
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

        if(64 - board.getTurnCount() >= moveOrderingHeight) {
            moveOrdering(cells, board, color, 4, eval, std::greater<i64>());
        }
        else if(64 - board.getTurnCount() >= ffhHeight) {
            ffhSort(cells, board, color);
        }
        else {
            // 通常のalpha beta
            return negaAlpha(board, color, alpha, beta);
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

    i64 negaAlpha(const Board &board, CellState color, i64 alpha, i64 beta) {
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
            return -negaAlpha(board, switchCellState(color), -beta, -alpha);
        }
        auto nextColor = switchCellState(color);
        for(const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);
            alpha = std::max(alpha, -negaAlpha(nextBoard, nextColor, -beta, -alpha));
            if(alpha >= beta)
                return beta;
        }

        return alpha;
    }

    void ffhSort(Cells &cells, const Board &board, CellState color) noexcept {
        std::array<i64, 64> order;
        auto nextColor = switchCellState(color);
        for(const auto &cell : cells) {
            auto nextBoard(board);
            nextBoard.putStone(color,cell);
            order[cell.toInt()] = nextBoard.getReversibleCount(nextColor);
        }

        std::sort(cells.begin(), cells.end(),
                [order](const CellType &cell1, const CellType &cell2) {
                return order[cell1.toInt()] < order[cell2.toInt()];
                });
    }

};

} // end crosswalk

#endif

