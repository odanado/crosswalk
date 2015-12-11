#include "MidGameAI.hpp"
#include "TableBaseEval.hpp"
#include "MidGameEval.hpp"
#include "EndGameAI.hpp"

#include <iostream>
#include <random>

using namespace crosswalk;
void put(Board &board, CellType cell, CellState color) {
    if(cell.getX() < 8) {
        if(color == CellState::BLACK)
            std::cout<<"black move: "<<cell.toString()<<std::endl;
        else
            std::cout<<"white move: "<<cell.toString()<<std::endl;

        board.putStone(color, cell);
    }
    else {
        if(color == CellState::BLACK)
            std::cout<<"black pass"<<std::endl;
        else
            std::cout<<"white pass"<<std::endl;
    }
}

void lastPlay(Board &board) {
    CellType cell(-1, -1);
    EndGameAI ai;
    while(!board.isFinished()) {
        cell = ai(board,CellState::BLACK);
        put(board, cell, CellState::BLACK);
        std::cout<<board.toString(true)<<std::endl;

        cell = ai(board,CellState::WHITE);
        put(board, cell, CellState::WHITE);
        std::cout<<board.toString(true)<<std::endl;
    }
}

template<class AI1, class AI2>
void test(Board board, AI1 ai1, AI2 ai2) {
    CellType cell(-1, -1);
    while(64 - board.getTurnCount() > 18) {
        cell = ai1(board, CellState::BLACK, 11);
        put(board, cell, CellState::BLACK);
        std::cout<<board.toString(true)<<std::endl;

        cell = ai2(board, CellState::WHITE, 11);
        put(board, cell, CellState::WHITE);
        std::cout<<board.toString(true)<<std::endl;
    }

    EndGameAI ai;
    ai(board, CellState::BLACK);
    std::cout << "score: "<<ai.getScore()<<std::endl;
    lastPlay(board);
    std::cout<<"black: "<<board.countStone(CellState::BLACK)<<std::endl;
    std::cout<<"white: "<<board.countStone(CellState::WHITE)<<std::endl;
}

void random(Board &board, u32 seed) {
    CellType cell(-1, -1);
    Cells cells;
    std::mt19937 mt(seed);

    while(board.getTurnCount() < 15) {
        cells = board.makeReversibleCells(CellState::BLACK);
        std::shuffle(cells.begin(), cells.end(), mt);
        board.putStone(CellState::BLACK, cells.front());

        cells = board.makeReversibleCells(CellState::WHITE);
        std::shuffle(cells.begin(), cells.end(), mt);
        board.putStone(CellState::WHITE, cells.front());
    }
}

int main() {
    MidGameAI<TableBaseEval> ai1;
    MidGameAI<MidGameEval> ai2;

    Board board;
    //random(board, 0);

    test(board, ai2, ai1);

    return 0;
}
