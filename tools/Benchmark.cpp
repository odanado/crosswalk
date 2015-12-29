#include "../src/Utils.hpp"
#include "../src/EndGameAI.hpp"
#include <cstdio>
#include <chrono>
#include <vector>

struct TestCase {
    crosswalk::Board board;
    crosswalk::CellState color;
    crosswalk::i64 score;
    TestCase(const crosswalk::Board &board, crosswalk::CellState color, crosswalk::i64 score) :
        board(board), color(color), score(score) {}
};

void init(std::vector<TestCase> &tests) {
    using namespace crosswalk;
    tests.push_back(TestCase(Board(0x008080C0C48C8080,0x0010783F3B737E79), CellState::BLACK, 38));

    tests.push_back(TestCase(Board(0x000080b09898bc3e, 0x207c7e4e64664000), CellState::BLACK, 34));
    tests.push_back(TestCase(Board(0x0010285838001800, 0x3c2c56a787fe047c), CellState::BLACK, -17));
    tests.push_back(TestCase(Board(0x00b0f0b3daa48081, 0x00000848245a7e7e), CellState::BLACK, 10));
    tests.push_back(TestCase(Board(0x0c1d0b170f592028, 0x2020746870261e10), CellState::BLACK, -20));

    tests.push_back(TestCase(Board(0x7c7831020e0f3c32, 0x0004cc7cf0f00000), CellState::WHITE, -2));
    tests.push_back(TestCase(Board(0x201cffe4c0ffbc74, 0x0000001b3f000000), CellState::WHITE, 54));
    tests.push_back(TestCase(Board(0x001407b3b7ce8400, 0x7c28784c48313800), CellState::WHITE, -22));
    tests.push_back(TestCase(Board(0x2401032d1b0d3d1e, 0x18387c52a4720000), CellState::WHITE, 14));
    // */
}

void benchmark(const crosswalk::Board &board, crosswalk::CellState color, crosswalk::EndGameAI &ai) {
    using namespace crosswalk;
    auto start = std::chrono::system_clock::now();
    auto cell = ai(board, color);
    auto end = std::chrono::system_clock::now();
    i64 diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    auto cache = ai.getCache();

    printf("ans: %s\n", cell.toString().c_str());
    printf("value: %lld\n", ai.getScore());
    printf("node: %llu\n",ai.getNodeCount());
    printf("leaf: %llu\n",ai.getLeafCount());
    printf("time: %lld [ms]\n",diff);
    printf("nps: %.2fk\n",1.0*ai.getNodeCount()/diff);
    printf("FFHHeight: %lld\n", ai.getFFHHeight());
    printf("cacheHeight: %lld\n", ai.getCacheHeight());
    printf("size: %lu\n",cache.size());
    printf("bucket_count: %lu\n",cache.bucket_count());
    printf("load_factor: %f\n",cache.load_factor());
    puts("\n");
}

void run(const crosswalk::Board &board, crosswalk::CellState color, crosswalk::i64 score) {
    using namespace crosswalk;
    printf("test case\n");
    printf("black: %016llx\n", board.getBitBoard(CellState::BLACK));
    printf("white: %016llx\n", board.getBitBoard(CellState::WHITE));
    printf("%s\n",board.toString().c_str());
    if(color == CellState::BLACK)
        printf("Move Black\n");
    else
        printf("Move White\n");

    printf("last score: %lld\n", score);
    printf("last count: %lld\n",64 - board.getTurnCount());

    EndGameAI ai;
    ai.setCacheHeight(6);
    ai.setFFHHeight(6);
    benchmark(board, color, ai);
}

int main() {
    static_assert(sizeof(std::size_t) == 8, "error");
    std::vector<TestCase> tests;
    init(tests);
    for(const auto &test : tests) {
        run(test.board, test.color, test.score);
    }
}

