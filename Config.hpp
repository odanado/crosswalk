#ifndef INCLUDE_CROSSWALK_CONFIG_HPP
#define INCLUDE_CROSSWALK_CONFIG_HPP

#include <cstdint>
#include <vector>
#include <utility>
#include <limits>

namespace crosswalk {

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using i32 = std::int32_t;

enum class CellState {
    NONE,
    BLACK,
    WHITE
};

template<typename T>
constexpr T maxValue() {
    return std::numeric_limits<T>::max() >> 2;
}
template<typename T>
constexpr T minValue() {
    return -maxValue<T>();
}

struct CellType {
private:
    u32 y,x;
public:
    CellType(u32 y, u32 x) :
        y(y),x(x) {}

    CellType(const CellType &rhs) noexcept {
        y = rhs.y;
        x = rhs.x;
    }
    
    u32 getY() const noexcept {
        return y;
    }
    u32 getX() const noexcept {
        return x;
    }

    void setY(u32 y) noexcept {
        this->y = y;
    }

    void setX(u32 x) noexcept {
        this->x = x;
    }

    bool operator<(const CellType &rhs) const noexcept {
        return y < rhs.y || (!(rhs.y < y) && x < rhs.x);
    }
    bool operator==(const CellType &rhs) const noexcept {
        return y == rhs.y && x == rhs.x;
    }
    
    u64 toInt() const noexcept { 
        return (y<<3) + x; 
    }

};

using Cells = std::vector<CellType>;

} // crosswalk

#endif

