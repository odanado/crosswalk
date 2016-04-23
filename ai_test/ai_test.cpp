#include <tuple>
#include <random>
#include <array>

#include "../src/MidGameAI.hpp"
#include "../src/EndGameAI.hpp"
#include "../src/TableBaseEval.hpp"
#include "../src/MidGameEval.hpp"
#include "../src/WeightEval.hpp"

using namespace crosswalk;
using namespace std;

template <class AI>
void put(AI &ai, Board &board, CellState &color, int depth) {
    auto cell = ai(board, color, depth);
    if (cell.vaild()) {
        board.putStone(color, cell);
    }
    color = switchCellState(color);
}
int lastPlay(Board board, CellState color) {
    EndGameAI ai;
    ai(board, color, board.getEmptyCount());

    return ai.getScore();
}

template <class AI1, class AI2>
int play(AI1 ai1, AI2 ai2, Board board) {
    CellState color = CellState::BLACK;
    while (!(board.getEmptyCount() < 16)) {
        put(ai1, board, color, 7);
        if (board.getEmptyCount() < 16) break;

        put(ai2, board, color, 7);
    }
    return lastPlay(board, color);
}

Board randomBoard(int n, int seed) {
    Board board;
    CellState color = CellState::BLACK;
    mt19937 rand(seed);
    for (int i = 0; i < n; i++) {
        auto cells = board.makeReversibleCells(color);
        shuffle(cells.begin(), cells.end(), rand);
        board.putStone(color, cells.front());
        color = switchCellState(color);
    }

    return board;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: ai_test playCount\n");
        return 1;
    }
    auto ai1 = MidGameAI<MidGameEval>();
    auto ai2 = MidGameAI<MidGameEval>();
    ai1.setNormalDFSDepth(4);
    ai2.setNormalDFSDepth(4);
    //ai2.setEval(WeightEval("../weight/SD"));
    array<tuple<int, int,int>, 2> cnt;
    int n = atoi(argv[1]);

    random_device rng;

    for (int i = 0; i < n; i++) {
        std::cerr << i << std::endl;
        auto board = randomBoard(4, rng());
        auto score = play(ai1, ai2, board);
        if(score > 0) ++get<0>(cnt[0]);
        else if(score < 0) ++get<1>(cnt[0]);
        else ++get<2>(cnt[0]);

        score = play(ai2, ai1, board);
        if(score > 0) ++get<0>(cnt[1]);
        else if(score < 0) ++get<1>(cnt[1]);
        else ++get<2>(cnt[1]);
    }

    for (int i = 0; i < 2; i++) {
        printf("win: %d, lose: %d, draw: %d\n", get<0>(cnt[i]), get<1>(cnt[i]), get<2>(cnt[i]));
    }

}
