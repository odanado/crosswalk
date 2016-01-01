﻿#ifndef INCLUDE_CROSSWALK_BOARD_HPP
#define INCLUDE_CROSSWALK_BOARD_HPP

#include "Config.hpp"
#include <cassert>
#include <utility>
#include <string>
#include <functional>

#include <xmmintrin.h>


#define SELECTIVE_ASSIGN(a, b, cond, notCond) (_mm256_or_si256(_mm256_and_si256((a),(cond)), _mm256_and_si256((b),(notCond))))

#ifdef _MSC_VER
#  include "nmmintrin.h"
#  define popcnt64(x) _mm_popcnt_u64(x)
#else
#  define popcnt64(x) __builtin_popcount(x) + __builtin_popcount(x >> 32)
#endif

namespace crosswalk {

class Board {
public:
    Board(u64 black = 0x810000000, u64 white = 0x1008000000) :
        black(black), white(white) {}

    Board& operator=(const Board &rhs) noexcept {
        black = rhs.black;
        white = rhs.white;

        return *this;
    }

    Board(const Board &rhs) noexcept {
        black = rhs.black;
        white = rhs.white;
    }

    bool operator==(const Board &rhs) const noexcept {
        return black == rhs.black && white == rhs.white;
    }

    u64 getBitBoard(CellState color) const noexcept {
        if(color == CellState::BLACK)
            return black;
        else
            return white;
    }

    i64 getTurnCount() const noexcept {
        return countStone(black) + countStone(white);
    }

    u64 countStone(CellState color) const noexcept {
        if(color == CellState::BLACK)
            return countStone(black);
        else
            return countStone(white);
    }

    bool existStone(CellState color, u64 y, u64 x) const noexcept {
        if(color == CellState::BLACK)
            return existStone(black, y, x);
        else
            return existStone(white, y, x);
    }

    Cells makeReversibleCells(CellState color) const noexcept {
        Cells cells;
        u64 reversiblePos;

        if(color == CellState::BLACK)
            reversiblePos = makeReversiblePos(black, white);
        else
            reversiblePos = makeReversiblePos(white, black);

        for(u32 k=0;k<64;++k) {
            if(reversiblePos>>k&1) {
                cells.emplace_back(k>>3,k&7);
            }
        }

        return std::move(cells);
    }

    i64 getReversibleCount(CellState color) const noexcept {
        u64 reversiblePos;
        if(color == CellState::BLACK)
            reversiblePos = makeReversiblePos(black, white);
        else
            reversiblePos = makeReversiblePos(white, black);

        return bitCount(reversiblePos);
    }

    bool putStone(CellState color, const CellType &cell) noexcept {
        return putStone(color, cell.getY(), cell.getX());
    }

    bool putStone(CellState color, u64 y, u64 x) noexcept {
        if(color == CellState::BLACK)
            return putStone(black, white, y, x);
        else
            return putStone(white, black, y, x);
    }

    bool isFinished() const noexcept {
        return !(makeReversiblePos(black, white) | makeReversiblePos(white, black));
    }

    bool empty(const CellType &cell) const noexcept {
        return empty(cell.getY(), cell.getX());
    }

    bool empty(u64 y, u64 x) const noexcept {
        return !(existStone(black, y, x) ||
                existStone(white, y, x));
    }

    std::string toString(bool axis = false) const noexcept {
        std::string ret;
        if(axis) {
            ret += "  ";
            for(char c = 'a'; c <= 'h'; c++)
                ret += std::string(1, c) + " ";
            ret += "\n";
        }
        for(u32 k=0;k<64;k++) {
            if(axis && k%8==0) {
                ret += std::string(1, static_cast<char>((k>>3)+'1')) + " ";
            }
            if(existStone(black, k>>3, k&7))
                ret += "x";
            else if(existStone(white, k>>3, k&7))
                ret += "o";
            else
                ret += ".";
            if(axis)
                ret += " ";

            if((k&7)==7)
                ret += "\n";
        }

        return ret;
    }


private:
    u64 black, white;
    u64 countStone(u64 bitBoard) const noexcept {
        return bitCount(bitBoard);
    }

    u64 bitCount(u64 bit) const noexcept {
        return popcnt64(bit);
    }

    bool existStone(u64 bitBoard, u64 y, u64 x) const noexcept {
        assert(0 <= y && y < 8);
        assert(0 <= x && x < 8);
        return (bitBoard >> ((y << 3) + x)) & 1;
    }

    void setStone(u64 &bitBoard, u64 y, u64 x) noexcept {
        assert(0 <= y && y < 8);
        assert(0 <= x && x < 8);
        bitBoard |= (1ULL << ((y << 3) + x));
    }

    void clearStone(u64 &bitBoard, u64 y, u64 x) noexcept {
        assert(0 <= y && y < 8);
        assert(0 <= x && x < 8);
        bitBoard &= (0xFFFFFFFFFFFFFFFF ^ (1ULL << ((y << 3) + x)));
    }

    bool putStone(u64 &myBitBoard, u64 &oppBitBoard, u64 y, u64 x) noexcept {
        u64 pos = u64(1) << ((y<<3)+x);
        u64 rev = makeReverseBit(myBitBoard, oppBitBoard, pos);
        if(rev == 0) 
            return false;

        myBitBoard ^= pos | rev;
        oppBitBoard ^= rev;

        assert((myBitBoard & oppBitBoard) == 0);

        return true;
    }


    u64 makeReverseBit(u64 black, u64 white, u64 pos) noexcept {
        u64 rev = 0;
        
        const __m256i black256 = _mm256_set_epi64x(black, black, black, black);
        const __m256i white256 = _mm256_set_epi64x(white, white, white, white);
        const __m256i pos256 = _mm256_set_epi64x(pos, pos, pos, pos);
        const __m256i shift256 = _mm256_set_epi64x(1, 8, 7, 9);
        const __m256i xormask256 = _mm256_set_epi64x(0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);

        __m256i bitmask256 = _mm256_set_epi64x(0xfefefefefefefefe, 0xffffffffffffffff, 0x7f7f7f7f7f7f7f7f, 0xfefefefefefefefe);
        __m256i cond256 = _mm256_set_epi64x(0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);
        __m256i notCond256 = _mm256_xor_si256(cond256, xormask256);

        __m256i rev256 = _mm256_setzero_si256();
        __m256i tmp256 = _mm256_setzero_si256();
        __m256i mask256;

        mask256 = _mm256_and_si256(_mm256_sllv_epi64(pos256, shift256), bitmask256);
        for (int i = 0; i < 8; i++) {
            cond256 = _mm256_andnot_si256(_mm256_cmpeq_epi64(_mm256_and_si256(mask256, white256), _mm256_setzero_si256()), cond256);
            notCond256 = _mm256_xor_si256(cond256, xormask256);
            tmp256 = SELECTIVE_ASSIGN(_mm256_or_si256(tmp256, _mm256_and_si256(cond256, mask256)), tmp256, cond256, notCond256);
            mask256 = SELECTIVE_ASSIGN(_mm256_and_si256(_mm256_sllv_epi64(mask256, shift256), bitmask256), mask256, cond256, notCond256);
        }

        cond256 = _mm256_cmpeq_epi64(_mm256_and_si256(mask256, black256), _mm256_setzero_si256());
        rev256 = _mm256_andnot_si256(cond256, tmp256);


        tmp256 = _mm256_setzero_si256();
        bitmask256 = _mm256_set_epi64x(0x7f7f7f7f7f7f7f7f, 0xffffffffffffffff, 0xfefefefefefefefe, 0x7f7f7f7f7f7f7f7f);
        cond256 = _mm256_set_epi64x(0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);
        notCond256 = _mm256_xor_si256(cond256, xormask256);

        mask256 = _mm256_and_si256(_mm256_srlv_epi64(pos256, shift256), bitmask256);
        for (int i = 0; i < 8; i++) {
            cond256 = _mm256_andnot_si256(_mm256_cmpeq_epi64(_mm256_and_si256(mask256, white256), _mm256_setzero_si256()), cond256);
            notCond256 = _mm256_xor_si256(cond256, xormask256);
            tmp256 = SELECTIVE_ASSIGN(_mm256_or_si256(tmp256, _mm256_and_si256(cond256, mask256)), tmp256, cond256, notCond256);
            mask256 = SELECTIVE_ASSIGN(_mm256_and_si256(_mm256_srlv_epi64(mask256, shift256), bitmask256), mask256, cond256, notCond256);
        }

        cond256 = _mm256_cmpeq_epi64(_mm256_and_si256(mask256, black256), _mm256_setzero_si256());
        rev256 = _mm256_or_si256(rev256, _mm256_andnot_si256(cond256, tmp256));

        rev |= rev256.m256i_u64[0];
        rev |= rev256.m256i_u64[1];
        rev |= rev256.m256i_u64[2];
        rev |= rev256.m256i_u64[3];

        return rev;
    }


    u64 makeReversiblePos(u64 black, u64 white) const noexcept {
        u64 blank = ~(black | white);
        u64 mobility=0;

        const __m256i white256 = _mm256_set_epi64x(white, white, white, white);
        const __m256i black256 = _mm256_set_epi64x(black, black, black, black);
        const __m256i blank256 = _mm256_set_epi64x(blank, blank, blank, blank);
        const __m256i mask256 = _mm256_set_epi64x(0x7e7e7e7e7e7e7e7e, 0x00ffffffffffff00, 0x007e7e7e7e7e7e00, 0x007e7e7e7e7e7e00);
        const __m256i shift256 = _mm256_set_epi64x(1, 8, 7, 9);
        const __m256i w256 = _mm256_and_si256(white256, mask256);
        __m256i t256, mobility256;

        t256 = _mm256_and_si256(w256, _mm256_sllv_epi64(black256, shift256));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_sllv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_sllv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_sllv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_sllv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_sllv_epi64(t256, shift256)));

        mobility256 = _mm256_and_si256(blank256, _mm256_sllv_epi64(t256, shift256));

        mobility |= mobility256.m256i_u64[0];
        mobility |= mobility256.m256i_u64[1];
        mobility |= mobility256.m256i_u64[2];
        mobility |= mobility256.m256i_u64[3];


        t256 = _mm256_and_si256(w256, _mm256_srlv_epi64(black256, shift256));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_srlv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_srlv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_srlv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_srlv_epi64(t256, shift256)));
        t256 = _mm256_or_si256(t256, _mm256_and_si256(w256, _mm256_srlv_epi64(t256, shift256)));

        mobility256 = _mm256_and_si256(blank256, _mm256_srlv_epi64(t256, shift256));

        mobility |= mobility256.m256i_u64[0];
        mobility |= mobility256.m256i_u64[1];
        mobility |= mobility256.m256i_u64[2];
        mobility |= mobility256.m256i_u64[3];
        
        return mobility;
    }

};
    

} // crosswalk

namespace std {
template <>
struct hash<crosswalk::Board>{
public:
    size_t operator()(const crosswalk::Board &board) const {
        using namespace crosswalk;
        size_t seed = 0;
        auto black_hash = hash<u64>()(board.getBitBoard(CellState::BLACK));
        auto white_hash = hash<u64>()(board.getBitBoard(CellState::WHITE));

        seed ^= black_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= white_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
}

#endif

