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
    std::array<i32, 10> pow3;  // 3のべき乗

    std::array<i32, 2> horizotal2;
    std::array<i32, 2> horizotal3;
    std::array<i32, 2> horizotal4;

    std::array<i32, 2> vertical2;
    std::array<i32, 2> vertical3;
    std::array<i32, 2> vertical4;

    std::array<i32, 4> diag4;
    std::array<i32, 4> diag5;
    std::array<i32, 4> diag6;
    std::array<i32, 4> diag7;
    std::array<i32, 2> diag8;

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

        diag4[0] = diag4[1] = diag4[2] = diag4[3] = 0;
        diag5[0] = diag5[1] = diag5[2] = diag5[3] = 0;
        diag6[0] = diag6[1] = diag6[2] = diag6[3] = 0;
        diag7[0] = diag7[1] = diag7[2] = diag7[3] = 0;
        diag8[0] = diag8[1] = 0;

        edge2X[0] = edge2X[1] = edge2X[2] = edge2X[3] = 0;

        corner3x3[0] = corner3x3[1] = corner3x3[2] = corner3x3[3] = 0;
        corner2x5[0] = corner2x5[1] = corner2x5[2] = corner2x5[3] = 0;
        corner2x5[4] = corner2x5[5] = corner2x5[6] = corner2x5[7] = 0;
    }

    i32 encode(u64 black, u64 white, u32 y, u32 x) const noexcept {
        if ((black >> ((y << 3) + x)) & 1)
            return 1;
        else if ((white >> ((y << 3) + x)) & 1)
            return 2;
        else
            return 0;
    }

    i32 normalize(i32 index, int length) const noexcept {
        i32 rindex = 0;
        std::array<i32, 10> line;

        for (int i = 0; i < length; i++) {
            line[i] = index % 3;
            index /= 3;
        }
        for (int i = 0; i < length; i++) {
            rindex = rindex * 3 + line[i];
        }

        return rindex;
    }

    i32 normalizeVector(i32 index) const noexcept {
        return std::min(index, normalize(index, 8));
    }

    i32 normalizeDiag4(i32 index) const noexcept {
        return std::min(index, normalize(index, 4));
    }
    i32 normalizeDiag5(i32 index) const noexcept {
        return std::min(index, normalize(index, 5));
    }
    i32 normalizeDiag6(i32 index) const noexcept {
        return std::min(index, normalize(index, 6));
    }
    i32 normalizeDiag7(i32 index) const noexcept {
        return std::min(index, normalize(index, 7));
    }
    i32 normalizeDiag8(i32 index) const noexcept {
        return std::min(index, normalize(index, 8));
    }
    i32 normalizeEdge2X(i32 index) const noexcept {
        return std::min(index, normalize(index, 10));
    }
    i32 normalizeCorner3x3(i32 index) const noexcept {
        i32 idx = index;
        i32 rindex = 0;
        std::array<i32, 9> line;
        for (std::size_t i = 0; i < 9; i++) {
            line[i] = idx % 3;
            idx /= 3;
        }
        // 対称軸で折り返す
        std::swap(line[1], line[3]);
        std::swap(line[2], line[6]);
        std::swap(line[5], line[7]);

        for (std::size_t i = 0; i < 9; i++) {
            rindex = 3 * rindex + line[i];
        }

        return std::min(index, rindex);
    }

 public:
    Feature() {
        pow3[0] = 1;
        for (std::size_t i = 1; i < pow3.size(); i++) {
            pow3[i] = pow3[i - 1] * 3;
        }
    }

    i32 getVector2Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += horizotal2[0] == index;
        ret += horizotal2[1] == index;
        ret += vertical2[0] == index;
        ret += vertical2[1] == index;

        return ret;
    }

    i32 getVector3Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += horizotal3[0] == index;
        ret += horizotal3[1] == index;
        ret += vertical3[0] == index;
        ret += vertical3[1] == index;

        return ret;
    }

    i32 getVector4Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += horizotal4[0] == index;
        ret += horizotal4[1] == index;
        ret += vertical4[0] == index;
        ret += vertical4[1] == index;

        return ret;
    }

    i32 getDiag4Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += diag4[0] == index;
        ret += diag4[1] == index;
        ret += diag4[2] == index;
        ret += diag4[3] == index;

        return ret;
    }

    i32 getDiag5Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += diag5[0] == index;
        ret += diag5[1] == index;
        ret += diag5[2] == index;
        ret += diag5[3] == index;

        return ret;
    }

    i32 getDiag6Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += diag6[0] == index;
        ret += diag6[1] == index;
        ret += diag6[2] == index;
        ret += diag6[3] == index;

        return ret;
    }

    i32 getDiag7Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += diag7[0] == index;
        ret += diag7[1] == index;
        ret += diag7[2] == index;
        ret += diag7[3] == index;

        return ret;
    }

    i32 getDiag8Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += diag8[0] == index;
        ret += diag8[1] == index;

        return ret;
    }

    i32 getEdge2XCount(i32 index) const noexcept {
        i32 ret = 0;
        ret += edge2X[0] == index;
        ret += edge2X[1] == index;
        ret += edge2X[2] == index;
        ret += edge2X[3] == index;

        return ret;
    }

    i32 getCorner3x3Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += corner3x3[0] == index;
        ret += corner3x3[1] == index;
        ret += corner3x3[2] == index;
        ret += corner3x3[3] == index;

        return ret;
    }

    i32 getCorner2x5Count(i32 index) const noexcept {
        i32 ret = 0;
        ret += corner2x5[0] == index;
        ret += corner2x5[1] == index;
        ret += corner2x5[2] == index;
        ret += corner2x5[3] == index;
        ret += corner2x5[4] == index;
        ret += corner2x5[5] == index;
        ret += corner2x5[6] == index;
        ret += corner2x5[7] == index;

        return ret;
    }

    void update(u64 black, u64 white) noexcept {
        init();
        for (std::size_t i = 0; i < BOARD_SIZE; i++) {
            horizotal2[0] += encode(black, white, 1, i) * pow3[i];
            horizotal3[0] += encode(black, white, 2, i) * pow3[i];
            horizotal4[0] += encode(black, white, 3, i) * pow3[i];
            horizotal4[1] +=
                encode(black, white, 4, BOARD_SIZE - i - 1) * pow3[i];
            horizotal3[1] +=
                encode(black, white, 5, BOARD_SIZE - i - 1) * pow3[i];
            horizotal2[1] +=
                encode(black, white, 6, BOARD_SIZE - i - 1) * pow3[i];

            vertical2[0] += encode(black, white, i, 6) * pow3[i];
            vertical3[0] += encode(black, white, i, 5) * pow3[i];
            vertical4[0] += encode(black, white, i, 4) * pow3[i];
            vertical4[1] +=
                encode(black, white, BOARD_SIZE - i - 1, 3) * pow3[i];
            vertical3[1] +=
                encode(black, white, BOARD_SIZE - i - 1, 2) * pow3[i];
            vertical2[1] +=
                encode(black, white, BOARD_SIZE - i - 1, 1) * pow3[i];

            diag8[0] += encode(black, white, i, BOARD_SIZE - i - 1) * pow3[i];
            diag8[1] +=
                encode(black, white, BOARD_SIZE - i - 1, BOARD_SIZE - i - 1) *
                pow3[i];

            if (i < BOARD_SIZE - 1) {
                diag7[0] +=
                    encode(black, white, i, BOARD_SIZE - i - 2) * pow3[i];
                diag7[1] += encode(black, white, BOARD_SIZE - i - 2,
                                   BOARD_SIZE - i - 1) *
                            pow3[i];
                diag7[2] +=
                    encode(black, white, BOARD_SIZE - i - 1, i + 1) * pow3[i];
                diag7[3] += encode(black, white, i + 1, i) * pow3[i];
            }

            if (i < BOARD_SIZE - 2) {
                diag6[0] +=
                    encode(black, white, i, BOARD_SIZE - i - 3) * pow3[i];
                diag6[1] += encode(black, white, BOARD_SIZE - i - 3,
                                   BOARD_SIZE - i - 1) *
                            pow3[i];
                diag6[2] +=
                    encode(black, white, BOARD_SIZE - i - 1, i + 2) * pow3[i];
                diag6[3] += encode(black, white, i + 2, i) * pow3[i];
            }

            if (i < BOARD_SIZE - 3) {
                diag5[0] +=
                    encode(black, white, i, BOARD_SIZE - i - 4) * pow3[i];
                diag5[1] += encode(black, white, BOARD_SIZE - i - 4,
                                   BOARD_SIZE - i - 1) *
                            pow3[i];
                diag5[2] +=
                    encode(black, white, BOARD_SIZE - i - 1, i + 3) * pow3[i];
                diag5[3] += encode(black, white, i + 3, i) * pow3[i];
            }

            if (i < BOARD_SIZE - 4) {
                diag4[0] +=
                    encode(black, white, i, BOARD_SIZE - i - 5) * pow3[i];
                diag4[1] += encode(black, white, BOARD_SIZE - i - 5,
                                   BOARD_SIZE - i - 1) *
                            pow3[i];
                diag4[2] +=
                    encode(black, white, BOARD_SIZE - i - 1, i + 4) * pow3[i];
                diag4[3] += encode(black, white, i + 4, i) * pow3[i];
            }
            edge2X[0] += encode(black, white, 0, i) * pow3[i + 1];
            edge2X[1] += encode(black, white, i, 7) * pow3[i + 1];
            edge2X[2] +=
                encode(black, white, 7, BOARD_SIZE - i - 1) * pow3[i + 1];
            edge2X[3] +=
                encode(black, white, BOARD_SIZE - i - 1, 0) * pow3[i + 1];
        }

        edge2X[0] += encode(black, white, 1, 1) * pow3[0] +
                     encode(black, white, 1, 6) * pow3[9];
        edge2X[1] += encode(black, white, 1, 6) * pow3[0] +
                     encode(black, white, 6, 6) * pow3[9];
        edge2X[2] += encode(black, white, 6, 6) * pow3[0] +
                     encode(black, white, 6, 1) * pow3[9];
        edge2X[3] += encode(black, white, 6, 1) * pow3[0] +
                     encode(black, white, 1, 1) * pow3[9];

        for (std::size_t i = 0; i < 9; i++) {
            corner3x3[0] += encode(black, white, i / 3, i % 3) * pow3[i];
            corner3x3[1] +=
                encode(black, white, i % 3, BOARD_SIZE - i / 3 - 1) * pow3[i];
            corner3x3[2] += encode(black, white, BOARD_SIZE - i / 3 - 1,
                                   BOARD_SIZE - i % 3 - 1) *
                            pow3[i];
            corner3x3[3] +=
                encode(black, white, BOARD_SIZE - i % 3 - 1, i / 3) * pow3[i];
        }

        for (std::size_t i = 0; i < 10; i++) {
            corner2x5[0] += encode(black, white, i / 5, i % 5) * pow3[i];
            corner2x5[1] +=
                encode(black, white, i % 5, BOARD_SIZE - i / 5 - 1) * pow3[i];
            corner2x5[2] += encode(black, white, BOARD_SIZE - i / 5 - 1,
                                   BOARD_SIZE - i % 5 - 1) *
                            pow3[i];
            corner2x5[3] +=
                encode(black, white, BOARD_SIZE - i % 5 - 1, i / 5) * pow3[i];

            corner2x5[4] += encode(black, white, i % 5, i / 5) * pow3[i];
            corner2x5[5] +=
                encode(black, white, i / 5, BOARD_SIZE - i % 5 - 1) * pow3[i];
            corner2x5[6] += encode(black, white, BOARD_SIZE - i % 5 - 1,
                                   BOARD_SIZE - i / 5 - 1) *
                            pow3[i];
            corner2x5[7] +=
                encode(black, white, BOARD_SIZE - i / 5 - 1, i % 5) * pow3[i];
        }

        for (std::size_t i = 0; i < 2; i++) {
            horizotal2[i] = normalizeVector(horizotal2[i]);
            horizotal3[i] = normalizeVector(horizotal3[i]);
            horizotal4[i] = normalizeVector(horizotal4[i]);
            vertical2[i] = normalizeVector(vertical2[i]);
            vertical3[i] = normalizeVector(vertical3[i]);
            vertical4[i] = normalizeVector(vertical4[i]);

            diag8[i] = normalizeDiag8(diag8[i]);
        }
        for (std::size_t i = 0; i < 4; i++) {
            diag4[i] = normalizeDiag4(diag4[i]);
            diag5[i] = normalizeDiag5(diag5[i]);
            diag6[i] = normalizeDiag6(diag6[i]);
            diag7[i] = normalizeDiag7(diag7[i]);

            edge2X[i] = normalizeEdge2X(edge2X[i]);
            corner3x3[i] = normalizeCorner3x3(corner3x3[i]);
            corner2x5[i] = std::min(corner2x5[i], corner2x5[i + 4]);
            corner2x5[i + 4] = corner2x5[i];
        }
    }

    i32 getVector2(i32 i) const noexcept {
        assert(0 <= i && i < 4);
        return i < 2 ? horizotal2[i] : vertical2[i & 1];
    }

    i32 getVector3(i32 i) const noexcept {
        assert(0 <= i && i < 4);
        return i < 2 ? horizotal3[i] : vertical3[i & 1];
    }

    i32 getVector4(i32 i) const noexcept {
        assert(0 <= i && i < 4);
        return i < 2 ? horizotal4[i] : vertical4[i & 1];
    }

    i32 getDiag4(i32 i) const noexcept {
        assert(0 <= i && i < diag4.size());
        return diag4[i];
    }

    i32 getDiag5(i32 i) const noexcept {
        assert(0 <= i && i < diag5.size());
        return diag5[i];
    }

    i32 getDiag6(i32 i) const noexcept {
        assert(0 <= i && i < diag6.size());
        return diag6[i];
    }

    i32 getDiag7(i32 i) const noexcept {
        assert(0 <= i && i < diag7.size());
        return diag7[i];
    }

    i32 getDiag8(i32 i) const noexcept {
        assert(0 <= i && i < diag8.size());
        return diag8[i];
    }

    i32 getEdge2X(i32 i) const noexcept {
        assert(0 <= i && i < edge2X.size());
        return edge2X[i];
    }

    i32 getCorner2x5(i32 i) const noexcept {
        assert(0 <= i && i < corner2x5.size());
        return corner2x5[i];
    }

    i32 getCorner3x3(i32 i) const noexcept {
        assert(i && i < corner3x3.size());
        return corner3x3[i];
    }
};

}  // namespace crosswalk

#endif
