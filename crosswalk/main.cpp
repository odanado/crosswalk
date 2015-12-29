#include "../src/MidGameAI.hpp"
#include "../src/MidGameEval.hpp"
#include "../src/EndGameAI.hpp"

#include <Windows.h>

#include <iostream>
#include <string>

void wait() {
    std::string s;
    std::cin.ignore();
    std::getline(std::cin, s);
}
using namespace crosswalk;

CellType next(const Board &board, CellState color) {
    if (64 - board.getTurnCount() <= 20) {
        EndGameAI ai;

        auto ret = ai(board, color);
        std::cout << "last: " << ai.getScore() << std::endl;

        return ret;
    }

    MidGameAI<MidGameEval> ai;
    return ai(board, color, 11);
}

void put(Board &board, CellType cell, CellState color) {
    if (cell.getX() < 8) {
        board.putStone(color, cell);

        std::cout << cell.toString() << " �ɑł��܂���." << std::endl;
    }
    else {
        std::cout << "�p�X���I������܂���." << std::endl;
    }
}


CellType input(Board board, CellState color) {
go:;
    std::cout << "����̎����͂��Ă�������. \n(�A���t�@�x�b�g�͏������A�p�X�Ȃ� pass �Ɠ���) > ";
    std::string in;
    std::cin >> in;
    if (in == "pass")
        return CellType(-1, -1);


    if (in.size() != 2 || !islower(in[0]) || !isdigit(in[1])) {
        std::cout << "���͂��s���ł�. ���͂��ꂽ����: " << in << std::endl;
        std::cout << "������x���͂��Ă�������." << std::endl << std::endl;
        goto go;

    }

    auto cell = CellType(in);

    if (!board.putStone(color, cell)) {
        std::cout << "�����ȓ��͂ł�. ���͂��ꂽ����: " << in << std::endl;
        std::cout << "������x���͂��Ă�������." << std::endl << std::endl;
        goto go;
    }

    return cell;
}

void gameWhite(Board &board) {
    CellState myColor = CellState::WHITE;

    CellType cell(-1, -1);
    while (!board.isFinished()) {

        {
            cell = input(board, switchCellState(myColor));
            put(board, cell, switchCellState(myColor));
            std::cout << board.toString(true) << std::endl;

            std::cout << std::string(25, '-') << std::endl << std::endl;
        }

        if (board.isFinished())
            break;

        {
            auto start = GetTickCount();
            cell = next(board, myColor);
            auto end = GetTickCount();
            i64 diff = end - start;

            put(board, cell, myColor);
            std::cout << board.toString(true) << std::endl;
            std::cout << "�v�l����: " << diff << " [ms]" << std::endl << std::endl;

            std::cout << std::string(25, '-') << std::endl << std::endl;
        }
    }
}

void gameBlack(Board &board) {
    CellState myColor = CellState::BLACK;

    CellType cell(-1, -1);
    while (!board.isFinished()) {
        {
            auto start = GetTickCount();
            cell = next(board, myColor);
            auto end = GetTickCount();
            i64 diff = end - start;

            put(board, cell, myColor);
            std::cout << board.toString(true) << std::endl;
            std::cout << "�v�l����: " << diff << " [ms]" << std::endl << std::endl;

            std::cout << std::string(25, '-') << std::endl << std::endl;
        }

        if (board.isFinished())
            break;

        {
            cell = input(board, switchCellState(myColor));
            put(board, cell, switchCellState(myColor));
            std::cout << board.toString(true) << std::endl;

            std::cout << std::string(25, '-') << std::endl << std::endl;
        }
    }
}

void game(Board &board, std::string in) {
    std::cout << "�Q�[���J�n�ł�." << std::endl;
    std::cout << std::string(25, '-') << std::endl << std::endl;

    if (in == "black") {
        gameBlack(board);
    }
    else if (in == "white") {
        gameWhite(board);
    }
}

void test() {
    Board board(0x008080C0C48C8080, 0x0010783F3B737E79);
    EndGameAI ai;
    auto start = GetTickCount();
    auto cell = ai(board, CellState::BLACK);
    auto end = GetTickCount();
    i64 diff = end - start;

    std::cout << cell.toString() << ": " << ai.getScore() << std::endl;
    std::cout << "�v�l����: " << diff << " [ms]" << std::endl << std::endl;
}

int main() {
    //test();
    Board board;
    std::string in;

go:;
    std::cout << "AI �̐F����͂��Ă������� (black or white) > ";
    std::cin >> in;

    if (in == "black" || in == "white") {
        game(board, in);
    }
    else {
        std::cout << "���͂��s���ł�. ���͂��ꂽ����: " << in << std::endl;
        std::cout << "������x���͂��Ă�������." << std::endl << std::endl;
        goto go;
    }


    std::cout << "�Q�[���I���ł�." << std::endl;
    std::cout << "black: " << board.countStone(CellState::BLACK) << std::endl;
    std::cout << "white: " << board.countStone(CellState::WHITE) << std::endl;

    std::cout << "�G���^�[�ŏI�����܂�." << std::endl;
    wait();

    return 0;
}
