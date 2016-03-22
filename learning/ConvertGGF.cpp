/*
 * GGFを学習用データセットに変換する
 * 標準入力からGGF形式の譜面を読み込み
 * 標準出力に変換したデータセットを書き出す
 * データセットの形式は/(\+|-)\d{2,}([a-g][1-8]|pa)+/
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "../src/Board.hpp"
#include "../src/EndGameAI.hpp"

crosswalk::Board parseBoard(const std::string &line) {
    crosswalk::u64 black, white;
    black = white = 0;

    // 譜面情報位置までシークする
    // +5 しているのは'BO[8 以下譜面' とBOと譜面までにある他の文字列を飛ばすため
    auto start = line.find("BO") + 5;

    int shift = 0;
    for (int i = start; shift != 64; i++) {
        if (line[i] == ' ') continue;
        if (line[i] == '*') black |= 1LL << shift;
        if (line[i] == 'O') white |= 1LL << shift;
        shift++;
    }

    return crosswalk::Board(black, white);
}

void parsePlay(const std::string &line, int &pos,
               std::vector<std::string> &cells) {
    if (line[pos + 1] == ';') return;
    // ](b|w)[ の分を飛ばす
    pos += 3;

    if (line[pos] == 'p' or line[pos] == 'P') {
        while (line[pos] != ']') pos++;
        cells.push_back("pa");
        parsePlay(line, pos, cells);
        return;
    } else if (isalpha(line[pos])) {
        auto cell = line.substr(pos, 2);
        if (isupper(cell.front())) cell.front() = tolower(cell.front());
        if ('a' <= cell[0] and cell[0] <= 'h') {
            while (line[pos] != ']') pos++;
            cells.push_back(cell);
            parsePlay(line, pos, cells);
            return;
        }
    }
    // ???
    std::cerr << line << std::endl;
    std::cerr << pos << std::endl;
    std::cerr << line[pos] << std::endl;
    std::cerr << line.substr(pos - 10, 21) << std::endl;
    assert(false);
}

std::vector<std::string> parsePlay(const std::string &line) {
    int pos = line.find("]B[");
    std::vector<std::string> cells;

    // 不完全な譜面
    if (pos == std::string::npos) return cells;

    assert(line.find("]B[") < line.find("]W["));
    parsePlay(line, pos, cells);
    return cells;
}

// 譜面を最終16手から完全読みを行って修正する
// それに加えて途中で放棄された譜面に対してはemptyなcells返す
// 黒にとっての最終スコアを返す
std::vector<std::string> correctPlay(const std::vector<std::string> &cells,
                                     int &lastScore) {
    crosswalk::Board board;
    std::vector<std::string> correctCells;
    auto color = crosswalk::CellState::BLACK;

    for (const auto &cell : cells) {
        if (cell != "pa") {
            board.putStone(color, cell);
        }
        correctCells.push_back(cell);
        color = crosswalk::switchCellState(color);

        if (board.getEmptyCount() == 16) {
            break;
        }
    }

    if (board.getEmptyCount() != 16) {
        // 不完全な譜面
        return std::vector<std::string>();
    }

    crosswalk::EndGameAI ai;
    while (!board.isFinished()) {
        auto cell = ai(board, color, board.getEmptyCount());
        if (cell.vaild()) {
            board.putStone(color, cell);
            correctCells.push_back(cell.toString());
        } else {
            correctCells.push_back("pa");
        }
        color = crosswalk::switchCellState(color);
    }
    lastScore = board.countStone(crosswalk::CellState::BLACK) -
                board.countStone(crosswalk::CellState::WHITE);

    return correctCells;
}

// 8x8オセロか判定する
bool is8x8(const std::string &line) {
    auto pos = line.find("BO") + 3;

    return line.substr(pos, 1) == "8";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cout.tie(0);
    std::string line;
    std::cout.fill('0');
    std::cout << std::internal << std::showpos;
    int cnt = 0;
    while (std::getline(std::cin, line)) {
        if (!is8x8(line)) continue;
        auto board = parseBoard(line);
        // 初期譜面ではないものを弾く
        if (!(board == crosswalk::Board())) continue;
        auto cells = parsePlay(line);

        int lastScore;
        auto correctCells = correctPlay(cells, lastScore);
        // 不完全な譜面だった
        if (correctCells.empty()) continue;

        std::cout << std::setw(3) << lastScore;
        for (const auto &cell : correctCells) std::cout << cell;
        std::cout << std::endl;
        cnt++;
        if (cnt % 100 == 0) std::cerr << cnt << std::endl;
    }
    return 0;
}
