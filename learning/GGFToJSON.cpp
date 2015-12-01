#include "../picojson/picojson.h"
#include "../Board.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>

template<class Int>
std::string toHex(Int n) {
    std::string ret;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(16) << std::hex << n;
    ss >> ret;
    return ret;
}

int64_t hexToInt(const std::string &hex) {
    std::stringstream ss;
    int64_t ret;
    ss << std::hex << hex;
    ss >> ret;

    return ret;
}

picojson::value parseBoard(const std::string &line) {
    picojson::object ret;
    int64_t black, white;
    black = white = 0;

    int start=0;
    for(int i=0;i<line.size();i++) {
        if(line.substr(i,2) == "BO") {
            start = i+5;
            break;
        }
    }
    int cnt=0;
    for(int i=start;i<line.size();i++) {
        if(cnt==64)
            break;
        if(line[i]!=' ') {
            if(line[i]=='*')
                black|=1LL<<cnt;
            if(line[i]=='O')
                white|=1LL<<cnt;
            cnt++;
        }
    }

    ret["black"] = picojson::value(toHex(black));
    ret["white"] = picojson::value(toHex(white));
    return picojson::value(ret);
}

picojson::value parsePlay(const std::string &line, int &pos) {
    std::string color;
    std::string cell;
    if(line[pos] == 'B' or line[pos] == 'b')
        color = "black";
    if(line[pos] == 'W' or line[pos] == 'w')
        color = "white";
    if(line[pos] == ';')
        return picojson::value();

    pos+=2;

    if(line[pos] == 'p' or line[pos] == 'P') {
        while(line[pos++] != ']') {
        }
        return parsePlay(line, pos);
    }
    else if(isalpha(line[pos])) {
        cell = line.substr(pos, 2);
        // アルファベット部分は小文字に合わせる
        if(isupper(cell[0])) {
            cell[0] = tolower(cell[0]);
        }
        if(!('a'<=cell[0]&&cell[0]<='h')) {
            std::cerr << cell << std::endl;
            std::cerr << line << std::endl;
            std::cerr << pos << std::endl;
            std::cerr << line.substr(pos-10,20) << std::endl;
            assert(false);
        }
        while(line[pos++] != ']') {
        }

        picojson::object ret;
        ret["turn"] = picojson::value(color);
        ret["cell"] = picojson::value(cell);

        return picojson::value(ret);
    }
    else {
        std::cerr << line << std::endl;
        std::cerr << pos << std::endl;
        std::cerr << line.substr(pos-10,20) << std::endl;
        assert(false);
    }
}

picojson::value parsePlays(const std::string &line) {
    picojson::object ret;
    int pos = -1;

    // 置き石の情報が来るまで読み飛ばし
    for(int i=0;i<line.size();i++) {
        if(line.substr(i,3) == "]B[") {
            pos = i+1;
            break;
        }
        if(line.substr(i,3) == "]W[") {
            pos = i+1;
            break;
        }
    }

    int turn = 1;
    if(pos != -1) {
        while(line[pos]!=';') {
            auto play = parsePlay(line, pos);
            if(!play.is<picojson::null>())
                ret[std::to_string(turn++)] = play;
        }
    }
    ret["turnCount"] = picojson::value(static_cast<double>(turn-1));
    return picojson::value(ret);
}

picojson::value getScore(picojson::value &&gameData) {
    picojson::object ret;
    auto&& data = gameData.get<picojson::object>();
    auto&& boardData = data["board"].get<picojson::object>();
    auto&& playsData = data["plays"].get<picojson::object>();

    auto board = crosswalk::Board(hexToInt(boardData["black"].get<std::string>()), hexToInt(boardData["white"].get<std::string>()));

    for(int i = 1;i <= static_cast<int>(playsData["turnCount"].get<double>());i++) {
        auto&& play = playsData[std::to_string(i)].get<picojson::object>();
        auto&& turn = play["turn"].get<std::string>();
        auto&& cell = play["cell"].get<std::string>();
        if(turn == "black") {
            board.putStone(crosswalk::CellState::BLACK, cell);
        }
        else {
            board.putStone(crosswalk::CellState::WHITE, cell);
        }
    }

    double black = board.countStone(crosswalk::CellState::BLACK);
    double white = board.countStone(crosswalk::CellState::WHITE);

    ret["black"] = picojson::value(black);
    ret["white"] = picojson::value(white);

    return picojson::value(ret);
}

picojson::value parse(const std::string &line) {
    picojson::object ret;
    ret["board"] = parseBoard(line);
    ret["plays"] = parsePlays(line);

    auto score = getScore(picojson::value(ret));
    ret["score"] = score;

    return picojson::value(ret);
}

// 8x8 のオセロか判定
bool is8x8(const std::string &line) {
    int pos = 0;
    for(int i=0;i<line.size();i++) {
        if(line.substr(i,2) == "BO") {
            pos = i+3;
            break;
        }
    }

    std::string num;
    while(line[pos] != ' ') {
        num += line[pos];
        ++pos;
    }

    return num == "8";
}

bool valid(picojson::value &gameData) {
    auto &&boardData = gameData.get<picojson::object>()["board"].get<picojson::object>();
    auto &&playsData = gameData.get<picojson::object>()["plays"].get<picojson::object>();
    auto board = crosswalk::Board(hexToInt(boardData["black"].get<std::string>()), hexToInt(boardData["white"].get<std::string>()));

    for(int i = 1;i <= static_cast<int>(playsData["turnCount"].get<double>());i++) {
        auto&& play = playsData[std::to_string(i)].get<picojson::object>();
        auto&& turn = play["turn"].get<std::string>();
        auto&& cell = play["cell"].get<std::string>();
        if(turn == "black") {
            board.putStone(crosswalk::CellState::BLACK, cell);
        }
        else {
            board.putStone(crosswalk::CellState::WHITE, cell);
        }
    }

    // 終局していないか、60手を過ぎず終わった試合は破棄
    return board.isFinished() || board.getTurnCount() >= 60;
}

int main() {
    std::string line;
    picojson::array gameDatas;

    while(std::getline(std::cin, line)) {
        if(!is8x8(line))
            continue;
        auto gameData = parse(line);
        if(!valid(gameData))
            continue;
        gameDatas.push_back(gameData);
    }

    std::cout << picojson::value(gameDatas).serialize() << std::endl;
    return 0;
}

