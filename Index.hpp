#ifndef INCLUDE_CROSSWALK_INDEX_HPP
#define INCLUDE_CROSSWALK_INDEX_HPP

#include "Config.hpp"
#include <array>
#include <iostream>

namespace crosswalk {
/**
 * BitBoard から Indexを生成するクラス
 * empty 0
 * black 1
 * white 2
 */
class Index {
    static constexpr i32 BOARD_SIZE = 8;
    std::array<i32, 9> pow3; // 3のべき乗
    std::array<i32, 6> horizontal;
    std::array<i32, 6> vertical;

    std::array<std::size_t, 9> diagSize; // 対角線の長さ

    std::array<i32, 9> diagLeft; // 左下向きの対角線のパターン
    std::array<i32, 9> diagLeftOrigin; // 左下の対角線の初期値

    std::array<i32, 9> diagRight; // 右下向きの対角線のパターン
    std::array<i32, 9> diagRightOrigin; // 右下向きの対角線の初期値
public:
    Index() {
        pow3[0] = 1;
        for(std::size_t i = 1;i < pow3.size(); i++) {
            pow3[i] = pow3[i-1] * 3;
        }

        for(std::size_t i = 0; i < diagSize.size(); i++) {
            if(i < 5) {
                diagSize[i] = i + 4;
                diagLeftOrigin[i] = i + 3;
                diagRightOrigin[i] = 4 - i;
            }
            else {
                diagSize[i] = 12 - i;
                diagLeftOrigin[i] = diagLeftOrigin[i - 1] + 8;
                diagRightOrigin[i] = diagRightOrigin[i - 1] + 8;
            }
        }
    }

    void update(i64 black, i64 white) noexcept {
        for(std::size_t i = 0;i < horizontal.size(); i++) {
            horizontal[i] = 0;
            vertical[i] = 0;
            for(std::size_t j = 0; j < BOARD_SIZE; j++) {
                if(black >> (j + ((i + 1) << 3)) & 1)
                    horizontal[i] += pow3[j];
                if(white >> (j + ((i + 1) << 3)) & 1)
                    horizontal[i] += 2 * pow3[j];

                if(black >> (i + 1 +(j << 3)) & 1)
                    vertical[i] += pow3[j];
                if(white>>(i + 1 + (j << 3)) & 1)
                    vertical[i] += 2 * pow3[j];
            }
        }

        for(std::size_t i = 0; i < diagSize.size(); i++) {
            diagLeft[i] = 0;
            diagRight[i] = 0;
            for(std::size_t j = 0; j < diagSize[i]; j++) {
                if(black>>(diagLeftOrigin[i] + j * 7) & 1)
                    diagLeft[i] += pow3[j];
                if(white>>(diagLeftOrigin[i] + j * 7) & 1)
                    diagLeft[i] += 2 * pow3[j];

                if(black >> (diagRightOrigin[i] + j * 9) & 1)
                    diagRight[i] += pow3[j];
                if(white >> (diagRightOrigin[i] + j * 9) & 1)
                    diagRight[i] += 2 * pow3[j];
            }
        }
    }

    i32 getHorizontal(std::size_t index) const noexcept {
        assert(index < horizontal.size());
        return horizontal[index];
    }

    i32 getVertical(std::size_t index) const noexcept {
        assert(index < vertical.size());
        return vertical[index];
    }

    i32 getDiagLeft(std::size_t index) const noexcept {
        assert(index < diagLeft.size());
        return diagLeft[index];
    }

    i32 getDiagRight(std::size_t index) const noexcept {
        assert(index < diagRight.size());
        return diagRight[index];
    }
};
} // namespace crosswalk

#endif

