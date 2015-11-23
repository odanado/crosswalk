#ifndef INCLUDE_CROSSWALK_BOARD_HPP
#define INCLUDE_CROSSWALK_BOARD_HPP

#include "Config.hpp"
#include <cassert>
#include <utility>
#include <string>

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

        for(u64 k=0;k<64;++k) {
            if(reversiblePos>>k&1) {
                cells.emplace_back(k>>3,k&7);
            }
        }

        return std::move(cells);
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

    std::string toString() const noexcept {
        std::string ret;
        for(u64 k=0;k<64;k++) {
            if(existStone(black, k>>3, k&7))
                ret += "x";
            else if(existStone(white, k>>3, k&7))
                ret += "o";
            else
                ret += ".";

            if((k&7)==7)
                ret += "\n";
        }

        return ret;
    }


private:
    u64 black, white;
    u64 countStone(u64 bitBoard) const noexcept {
        return __builtin_popcount(bitBoard) + __builtin_popcount(bitBoard >> 32);
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
        u64 rev=0,tmp,mask;

        // 右
        tmp=0;
        mask = (pos >> 1) & 0x7f7f7f7f7f7f7f7f;
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask >> 1) & 0x7f7f7f7f7f7f7f7f;
        }
        if(mask & black) rev |= tmp;

        // 左
        tmp=0;
        mask = (pos << 1) & 0xfefefefefefefefe;
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask << 1) & 0xfefefefefefefefe;
        }
        if(mask & black) rev |= tmp;

        // 上
        tmp=0;
        mask = (pos << 8);
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask << 8);
        }
        if(mask & black) rev |= tmp;

        // 下
        tmp=0;
        mask = (pos >> 8);
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask >> 8);
        }
        if(mask & black) rev |= tmp;

        // 右上
        tmp=0;
        mask = (pos << 7) & 0x7f7f7f7f7f7f7f7f;
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask << 7) & 0x7f7f7f7f7f7f7f7f;
        }
        if(mask & black) rev |= tmp;

        // 左上
        tmp=0;
        mask = (pos << 9) & 0xfefefefefefefefe;
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask << 9) & 0xfefefefefefefefe;
        }
        if(mask & black) rev |= tmp;

        // 右下
        tmp=0;
        mask = (pos >> 9) & 0x7f7f7f7f7f7f7f7f;
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask >> 9) & 0x7f7f7f7f7f7f7f7f;
        }
        if(mask & black) rev |= tmp;

        // 左下
        tmp=0;
        mask = (pos >> 7) & 0xfefefefefefefefe;
        while(mask && (mask & white)) {
            tmp |= mask;
            mask = (mask >> 7) & 0xfefefefefefefefe;
        }
        if(mask & black) rev |= tmp;


        return rev;
    }


    u64 makeReversiblePos(u64 black, u64 white) const noexcept {
        u64 blank = ~(black | white);
        u64 mobility=0,t,w;

        // 右
        w = white & 0x7e7e7e7e7e7e7e7e; 
        t = w & (black << 1);
        t |= w & (t << 1); t |= w & (t << 1); t |= w & (t << 1);
        t |= w & (t << 1); t |= w & (t << 1);
        mobility |= blank & (t << 1);

        // 左
        w = white & 0x7e7e7e7e7e7e7e7e; 
        t = w & (black >> 1);
        t |= w & (t >> 1); t |= w & (t >> 1); t |= w & (t >> 1);
        t |= w & (t >> 1); t |= w & (t >> 1);
        mobility |= blank & (t >> 1);

        // 上
        w = white & 0x00ffffffffffff00;
        t = w & (black << 8);
        t |= w & (t << 8); t |= w & (t << 8); t |= w & (t << 8);
        t |= w & (t << 8); t |= w & (t << 8);
        mobility |= blank & (t << 8);

        // 下
        w = white & 0x00ffffffffffff00;
        t = w & (black >> 8);
        t |= w & (t >> 8); t |= w & (t >> 8); t |= w & (t >> 8);
        t |= w & (t >> 8); t |= w & (t >> 8);
        mobility |= blank & (t >> 8);

        // 右上
        w = white & 0x007e7e7e7e7e7e00;
        t = w & (black << 7);
        t |= w & (t << 7); t |= w & (t << 7); t |= w & (t << 7);
        t |= w & (t << 7); t |= w & (t << 7);
        mobility |= blank & (t << 7);

        // 左上
        w = white & 0x007e7e7e7e7e7e00;
        t = w & (black << 9);
        t |= w & (t << 9); t |= w & (t << 9); t |= w & (t << 9);
        t |= w & (t << 9); t |= w & (t << 9);
        mobility |= blank & (t << 9);

        // 右下
        w = white & 0x007e7e7e7e7e7e00;
        t = w & (black >> 9);
        t |= w & (t >> 9); t |= w & (t >> 9); t |= w & (t >> 9);
        t |= w & (t >> 9); t |= w & (t >> 9);
        mobility |= blank & (t >> 9);

        // 左下
        w = white & 0x007e7e7e7e7e7e00;
        t = w & (black >> 7);
        t |= w & (t >> 7); t |= w & (t >> 7); t |= w & (t >> 7);
        t |= w & (t >> 7); t |= w & (t >> 7);
        mobility |= blank & (t >> 7);

        return mobility;
    }

};
    
} // crosswalk

#endif
