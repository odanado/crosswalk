#ifndef INCLUDE_CROSSWALK_TRANSPOSITION_TABLE_HPP
#define INCLUDE_CROSSWALK_TRANSPOSITION_TABLE_HPP

#include "Board.hpp"

#include <unordered_map>

namespace crosswalk {

class AlphaBetaWindow {
private:
    i64 alpha, beta;
public:
    AlphaBetaWindow(i64 alpha=minValue<i64>(), i64 beta=maxValue<i64>()) :
        alpha(alpha), beta(beta) {}

    i64 getAlpha() const noexcept {
        return alpha;
    }
    void setAlpha(i64 alpha) noexcept {
        this->alpha = alpha;
        assert(this->alpha <= this->beta);
    }

    i64 getBeta() const noexcept {
        return beta;
    }
    void setBeta(i64 beta) noexcept {
        this->beta = beta;
        assert(this->alpha <= this->beta);
    }

};

class TranspositionTable : public std::unordered_map<Board, AlphaBetaWindow> {
private:
    // 登録数の上限
    i64 registerSize;
public:
    TranspositionTable() : 
        registerSize(1LL << 28) {}
    void update(const Board &board, i64 alpha, i64 beta) noexcept {
        auto &window = operator[](board);
        window.setAlpha(std::max(window.getAlpha(), alpha));
        window.setBeta(std::min(window.getBeta(), beta));
        /*
        if(count(board)) {
            auto &window = operator[](board);
            window.setAlpha(std::max(window.getAlpha(), alpha));
            window.setBeta(std::min(window.getBeta(), beta));
        }
        else {
            emplace(alpha, beta);
        }
        */
    }

};

} // end crosswalk

#endif

