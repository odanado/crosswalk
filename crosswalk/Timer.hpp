#ifndef INCLUDE_CROSSWALK_TIMER_HPP
#define INCLUDE_CROSSWALK_TIMER_HPP

#ifdef MSC_VER
#include <Windows.h>
int getMilliSec() { return GetTickCount(); }
#else
#include <chrono>
auto getMilliSec()
    -> decltype(std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() -
                    std::chrono::time_point<std::chrono::system_clock>())
                    .count()) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() -
               std::chrono::time_point<std::chrono::system_clock>())
        .count();
}
#endif
#endif
