#ifndef INCLUDE_CROSSWALK_FEATURE_HPP
#define INCLUDE_CROSSWALK_FEATURE_HPP

#include "Config.hpp"
#include <array>

namespace crosswalk {
/**
 * BitBoard から Feature(特徴量)を生成するクラス
 * empty 0
 * black 1
 * white 2
 */
class Feature {
static constexpr i32 BOARD_SIZE = 8;
std::array<i32, 10> pow3; // 3のべき乗

std::array<i32, 2> horizotal2;
std::array<i32, 2> horizotal3;
std::array<i32, 2> horizotal4;

std::array<i32, 2> vertical2;
std::array<i32, 2> vertical3;
std::array<i32, 2> vertical4;

std::array<i32, 4> diagonal4;
std::array<i32, 4> diagonal5;
std::array<i32, 4> diagonal6;
std::array<i32, 4> diagonal7;
std::array<i32, 2> diagonal8;

std::array<i32, 4> edge2X;

std::array<i32, 4> corner3x3;
std::array<i32, 8> corner2x5;


void init() {
        horizotal2[0] = horizotal2[1] = 0;
        horizotal3[0] = horizotal3[1] = 0;
        horizotal4[0] = horizotal4[1] = 0;

        vertical2[0] = vertical2[1] = 0;
        vertical3[0] = vertical3[1] = 0;
        vertical4[0] = vertical4[1] = 0;

        diagonal4[0] = diagonal4[1] = diagonal4[2] = diagonal4[3] = 0;
        diagonal5[0] = diagonal5[1] = diagonal5[2] = diagonal5[3] = 0;
        diagonal6[0] = diagonal6[1] = diagonal6[2] = diagonal6[3] = 0;
        diagonal7[0] = diagonal7[1] = diagonal7[2] = diagonal7[3] = 0;
        diagonal8[0] = diagonal8[1] = 0;

        edge2X[0] = edge2X[1] = edge2X[2] = edge2X[3] = 0;

        corner3x3[0] = corner3x3[1] = corner3x3[2] = corner3x3[3] = 0;
        corner2x5[0] = corner2x5[1] = corner2x5[2] = corner2x5[3] = 0;
        corner2x5[4] = corner2x5[5] = corner2x5[6] = corner2x5[7] = 0;
}

i32 encode(u64 black, u64 white, u32 y, u32 x) const noexcept {
    if(black >> ((y << 3) + x))
        return 1;
    else if(white >> ((y << 3) + x))
        return 2;
    else
        return 0;
}

public:
    Feature() {
        pow3[0] = 1;
        for(std::size_t i = 1;i < pow3.size(); i++) {
            pow3[i] = pow3[i-1] * 3;
        }
    }
    void update(u64 black, u64 white) noexcept {
        init();
        for(std::size_t i = 0; i < BOARD_SIZE; i++) {
            horizotal2[0] += encode(black, white, 1, i) * pow3[i];
            horizotal3[0] += encode(black, white, 2, i) * pow3[i];
            horizotal4[0] += encode(black, white, 3, i) * pow3[i];
            horizotal4[1] += encode(black, white, 4, i) * pow3[i];
            horizotal3[1] += encode(black, white, 5, i) * pow3[i];
            horizotal2[1] += encode(black, white, 6, i) * pow3[i];

            vertical2[0] += encode(black, white, i, 1) * pow3[i];
            vertical3[0] += encode(black, white, i, 2) * pow3[i];
            vertical4[0] += encode(black, white, i, 3) * pow3[i];
            vertical4[1] += encode(black, white, i, 4) * pow3[i];
            vertical3[1] += encode(black, white, i, 5) * pow3[i];
            vertical2[1] += encode(black, white, i, 6) * pow3[i];

            diagonal8[0] += encode(black, white, i, BOARD_SIZE - i - 1) * pow3[i];
            diagonal8[1] += encode(black, white, BOARD_SIZE - i - 1, BOARD_SIZE - i - 1) * pow3[i];
            
            if(i < BOARD_SIZE - 1) {
                diagonal7[0] += encode(black, white, i, BOARD_SIZE - i - 2) * pow3[i];
                diagonal7[1] += encode(black, white, BOARD_SIZE - i - 2, BOARD_SIZE - i - 1) * pow3[i];
                diagonal7[2] += encode(black, white, BOARD_SIZE - i - 1, i + 1) * pow3[i];
                diagonal7[3] += encode(black, white, i + 1, i) * pow3[i];
            }
            
            if(i < BOARD_SIZE - 2) {
                diagonal6[0] += encode(black, white, i, BOARD_SIZE - i - 3) * pow3[i];
                diagonal6[1] += encode(black, white, BOARD_SIZE - i - 3, BOARD_SIZE - i - 1) * pow3[i];
                diagonal6[2] += encode(black, white, BOARD_SIZE - i - 1, i + 2) * pow3[i];
                diagonal6[3] += encode(black, white, i + 2, i) * pow3[i];
            }

            if(i < BOARD_SIZE - 3) {
                diagonal5[0] += encode(black, white, i, BOARD_SIZE - i - 4) * pow3[i];
                diagonal5[1] += encode(black, white, BOARD_SIZE - i - 4, BOARD_SIZE - i - 1) * pow3[i];
                diagonal5[2] += encode(black, white, BOARD_SIZE - i - 1, i + 3) * pow3[i];
                diagonal5[3] += encode(black, white, i + 3, i) * pow3[i];
            }

            if(i < BOARD_SIZE - 4) {
                diagonal4[0] += encode(black, white, i, BOARD_SIZE - i - 5) * pow3[i];
                diagonal4[1] += encode(black, white, BOARD_SIZE - i - 5, BOARD_SIZE - i - 1) * pow3[i];
                diagonal4[2] += encode(black, white, BOARD_SIZE - i - 1, i + 4) * pow3[i];
                diagonal4[3] += encode(black, white, i + 4, i) * pow3[i];
            }
            edge2X[0] += encode(black, white, 0, i);
            edge2X[1] += encode(black, white, i, 7);
            edge2X[2] += encode(black, white, 7, BOARD_SIZE - i -1);
            edge2X[3] += encode(black, white, BOARD_SIZE - i - 1, 0);
        }

        edge2X[0] += encode(black, white, 1, 1) * pow3[8] + encode(black, white, 1, 6) * pow3[9];
        edge2X[1] += encode(black, white, 1, 6) * pow3[8] + encode(black, white, 6, 6) * pow3[9];
        edge2X[2] += encode(black, white, 6, 6) * pow3[8] + encode(black, white, 6, 1) * pow3[9];
        edge2X[3] += encode(black, white, 6, 1) * pow3[8] + encode(black, white, 1, 1) * pow3[9];

        for(std::size_t i = 0; i < 9; i++) {
            corner3x3[0] += encode(black, white, i / 3, i % 3) * pow3[i];
            corner3x3[1] += encode(black, white, i % 3, BOARD_SIZE - i / 3 - 1) * pow3[i];
            corner3x3[2] += encode(black, white, BOARD_SIZE - i / 3 - 1, BOARD_SIZE - i % 3 - 1) * pow3[i];
            corner3x3[3] += encode(black, white, BOARD_SIZE - i % 3 - 1, i / 3) * pow3[i];
        }

        for(std::size_t i = 0; i < 10; i++) {
            corner2x5[0] += encode(black, white, i / 5, i % 5) * pow3[i];
            corner2x5[1] += encode(black, white, i % 5, BOARD_SIZE - i / 5 - 1) * pow3[i];
            corner2x5[2] += encode(black, white, BOARD_SIZE - i / 5 - 1, BOARD_SIZE - i % 5 - 1) * pow3[i];
            corner2x5[3] += encode(black, white, BOARD_SIZE - i % 5 - 1, i / 5) * pow3[i];

            corner2x5[4] += encode(black, white, i % 5, i / 5) * pow3[i];
            corner2x5[5] += encode(black, white, i / 5, BOARD_SIZE - i % 5 - 1) * pow3[i];
            corner2x5[6] += encode(black, white, BOARD_SIZE - i % 5 - 1, BOARD_SIZE - i / 5 - 1) * pow3[i];
            corner2x5[7] += encode(black, white, BOARD_SIZE - i / 5 - 1, i % 5) * pow3[i];
        }

    }
};

} // namespace crosswalk

#endif

