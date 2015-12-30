#ifndef INCLUDE_CROSSWALK_TRANSPOSITION_TABLE_HPP
#define INCLUDE_CROSSWALK_TRANSPOSITION_TABLE_HPP

#include "Board.hpp"
#include "Config.hpp"
#include "NodeState.hpp"

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

class TranspositionTable : public std::unordered_map<NodeState, AlphaBetaWindow> {
private:
    // 登録数の上限
    std::size_t registerSize;
public:
    TranspositionTable() : 
        registerSize(1LL << 24) {}
    void update(const NodeState &nodeState, i64 alpha, i64 beta) noexcept {

        if (this->count(nodeState) != 0) {
            auto &window = operator[](nodeState);
            window.setAlpha(std::max(window.getAlpha(), alpha));
            window.setBeta(std::min(window.getBeta(), beta));
        }
        else if (size() <= registerSize) {
            this->emplace(nodeState, AlphaBetaWindow(alpha, beta));
        }

    }

};

} // end crosswalk

#endif

