#ifndef INCLUDE_CROSSWALK_AI_HPP
#define INCLUDE_CROSSWALK_AI_HPP

#include "Board.hpp"
#include "Utils.hpp"
#include "TranspositionTable.hpp"

namespace crosswalk {

template<class Eval, class MoveOrdering>
class AI {
public:
    CellType operator()(const Board &board, CellState color, i64 depth) noexcept {
        nodeCount = 0;
        leafCount = 0;

        CellType result(-1, -1);

        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();
        i64 a = alpha;
        i64 b = beta;
        i64 maxValue = minValue<i64>();
        bool first = true;

        myColor = color;
        auto cells = board.makeReversibleCells(color);

        moveOrdering(cells, board, color);

        for (const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);

            i64 evalValue = -dfs(nextBoard, switchCellState(color), -b, -a, depth - 1);
            if (a < evalValue && evalValue < beta && !first)
                a = -dfs(nextBoard, switchCellState(color), -beta, -evalValue, depth - 1);

            a = std::max(a, evalValue);
            b = a + 1;
            first = false;

            if (maxValue < a) {
                maxValue = a;
                result = cell;
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

    i64 getNormalDFSDepth() const noexcept {
        return normalDFSDepth;
    }

    void setNormalDFSDepth(i64 normalDFSDepth) noexcept {
        this->normalDFSDepth = normalDFSDepth;
    }

    const TranspositionTable& getCache() const noexcept {
        return cache;
    }

    void setEval(const Eval &eval) {
        this->eval = eval;
    }
private:
    Eval eval;
    MoveOrdering moveOrdering;

    i64 score;
    i64 normalDFSDepth; // Ç±ÇÃê[Ç≥à»â∫Ç…Ç»ÇÈÇ∆í èÌÇÃdfs

    i64 nodeCount;
    i64 leafCount;

    CellState myColor;
    TranspositionTable cache;

    i64 dfs(const Board &board, CellState color, i64 alpha, i64 beta, i64 depth) noexcept {
        assert(alpha <= beta);
        ++nodeCount;
        if (board.isFinished()) {
            ++leafCount;
            if (color == myColor)
                return eval(board, myColor);
            else
                return -eval(board, myColor);
        }

        if (depth == normalDFSDepth) {
            // í èÌÇÃalpha beta
            return negaAlpha(board, color, alpha, beta, depth);
        }

        auto cells = board.makeReversibleCells(color);

        if (cells.empty()) {
            i64 score = -dfs(board, switchCellState(color), -beta, -alpha, depth);
            if (beta <= score)
                cache.update(NodeState(board, color), score, maxValue<i64>());
            else if (score <= alpha)
                cache.update(NodeState(board, color), minValue<i64>(), score);
            else
                cache.update(NodeState(board, color), score, score);

            return score;
        }

        if (cache.count(NodeState(board, color))) {
            const auto &window = cache[NodeState(board, color)];
            if (window.getBeta() <= alpha)
                return window.getBeta();
            if (window.getAlpha() >= beta)
                return window.getAlpha();
            if (window.getAlpha() == window.getBeta())
                return window.getAlpha();

            alpha = std::max(alpha, window.getAlpha());
            beta = std::min(beta, window.getBeta());
            assert(alpha <= beta);
        }

        moveOrdering(cells, board, color);

        i64 value;
        i64 a = alpha;
        i64 b = beta;
        bool first = true;

        for (const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);

            value = -dfs(nextBoard, switchCellState(color), -b, -a, depth - 1);
            if (a < value && value < beta && !first) a = -dfs(nextBoard, switchCellState(color), -beta, -value, depth - 1);
            a = std::max(a, value);
            if (a >= beta) {
                cache.update(NodeState(board, color), a, maxValue<i64>());
                return a;
            }
            b = a + 1;
            first = false;
        }

        if (a > alpha) {
            cache.update(NodeState(board, color), a, a);
        }
        else {
            cache.update(NodeState(board, color), minValue<i64>(), a);
        }


        return a;
    }

    i64 negaAlpha(const Board &board, CellState color, i64 alpha, i64 beta, i64 depth) {
        assert(alpha <= beta);
        ++nodeCount;
        if (depth == 0 || board.isFinished()) {
            ++leafCount;
            if (color == myColor)
                return eval(board, myColor);
            else
                return -eval(board, myColor);
        }

        auto cells = board.makeReversibleCells(color);
        if (cells.empty()) {
            return -negaAlpha(board, switchCellState(color), -beta, -alpha, depth);
        }
        auto nextColor = switchCellState(color);
        for (const auto &cell : cells) {
            auto nextBoard = board;
            nextBoard.putStone(color, cell);
            alpha = std::max(alpha, -negaAlpha(nextBoard, nextColor, -beta, -alpha, depth - 1));
            if (alpha >= beta)
                return beta;
        }

        return alpha;
    }

};

} // end crosswalk

#endif
