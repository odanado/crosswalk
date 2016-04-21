#ifndef INCLUDE_CROSSWALK_WEIGHT_HPP
#define INCLUDE_CROSSWALK_WEIGHT_HPP

#include <array>
#include <vector>

namespace crosswalk {

template <class T>
struct Weight {
    std::vector<T> vector2;
    std::vector<T> vector3;
    std::vector<T> vector4;
    std::vector<T> diag4;
    std::vector<T> diag5;
    std::vector<T> diag6;
    std::vector<T> diag7;
    std::vector<T> diag8;
    std::vector<T> edge2X;
    std::vector<T> corner3x3;
    std::vector<T> corner2x5;
    T mobility;
    T parity;

    Weight()
        : vector2(std::vector<T>(6561)),
          vector3(std::vector<T>(6561)),
          vector4(std::vector<T>(6561)),
          diag4(std::vector<T>(81)),
          diag5(std::vector<T>(243)),
          diag6(std::vector<T>(729)),
          diag7(std::vector<T>(2187)),
          diag8(std::vector<T>(6561)),
          edge2X(std::vector<T>(59049)),
          corner3x3(std::vector<T>(19683)),
          corner2x5(std::vector<T>(59049)),
          mobility(0),
          parity(0) {}
};

}  // crosswalk

#endif
