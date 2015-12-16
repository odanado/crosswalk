#include "../picojson/picojson.h"
#include "../Board.hpp"
#include "../EndGameAI.hpp"

#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>

int64_t hexToInt(const std::string &hex) {
    std::stringstream ss;
    int64_t ret;
    ss << std::hex << hex;
    ss >> ret;

    return ret;
}

template<class Int>
std::string toHex(Int n) {
    std::string ret;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(16) << std::hex << n;
    ss >> ret;
    return ret;
}

crosswalk::Board getBoard(picojson::value gameData, int count, crosswalk::CellState &color) {
    using namespace crosswalk;
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
            color = CellState::WHITE;
        }
        else {
            board.putStone(crosswalk::CellState::WHITE, cell);
            color = CellState::BLACK;
        }

        if(board.getTurnCount() == count) {
            break;
        }
    }

    return board;
}

int main(int argc, char *argv[]) {
    if(argc < 1) {
        std::cout << "error " << std::endl;
        return 1;
    }
    int count = stoi(std::string(argv[1]));
    int depth = stoi(std::string(argv[2]));
    int seed = 0;
    if(argc > 3) {
        seed = stoi(std::string(argv[3]));
    }

    std::cout << "count: " << count << std::endl;
    std::cout << "depth: " << depth << std::endl;
    std::cout << "seed: " << seed << std::endl;

    using namespace crosswalk;
    picojson::value json;
    std::cin >> json;

    auto &&gameDatas = json.get<picojson::array>();

    std::mt19937 mt(seed);
    std::shuffle(gameDatas.begin(), gameDatas.end(), mt);

    std::cout << "start" << std::endl;

    int timeSum, nodeSum, leafSum;
    double npsSum;
    timeSum = npsSum = nodeSum = leafSum = 0;

    for(std::size_t i = 0; i < count; i++) {
        const auto &gameData = gameDatas[i];
        CellState color;
        auto board = getBoard(gameData, 64 - depth, color);
        
        EndGameAI ai;
        auto start = std::chrono::system_clock::now();
        auto cell = ai(board, color);
        auto end = std::chrono::system_clock::now();
        i64 diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        timeSum += diff;
        npsSum += 1.0 * ai.getNodeCount() / diff;
        nodeSum += ai.getNodeCount();
        leafSum += ai.getLeafCount();
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "time average: " << 1.0 * timeSum / count << " [ms]" << std::endl;
    std::cout << "nps average: " << 1.0 * npsSum / count << std::endl;
    std::cout << "node count average: " << 1.0 * nodeSum / count << std::endl;
    std::cout << "leaf count average: " << 1.0 * leafSum / count << std::endl;

}
