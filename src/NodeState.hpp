#ifndef INCLUDE_CROSSWALK_NODESTATE_HPP
#define INCLUDE_CROSSWALK_NODESTATE_HPP

#include "Board.hpp"
#include "Config.hpp"

namespace crosswalk {

class NodeState {
private:
    Board board;
    CellState color;
public:
    NodeState(const Board &board, CellState color) :
        board(board), color(color) {
    }

    std::size_t hash() const {
        size_t seed = 0;
        seed ^= getBoardHash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= getColorHash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

    std::size_t getBoardHash() const noexcept {
        return std::hash<crosswalk::Board>()(board);
    }

    std::size_t getColorHash() const noexcept {
        return color == CellState::BLACK ? 0 : 1;
    }

    bool operator==(const NodeState &rhs) const {
        return board == rhs.board && color == rhs.color;
    }


};

} // end crosswalk

namespace std {
template <>
struct hash<crosswalk::NodeState> {
public:
    size_t operator()(const crosswalk::NodeState &nodeState) const {
        return nodeState.hash();
    }
};
}

#endif