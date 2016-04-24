#ifndef INCLUDE_CROSSWALK_WEIGHT_EVAL_HPP
#define INCLUDE_CROSSWALK_WEIGHT_EVAL_HPP

#include <string>
#include <array>
#include <fstream>
#include <sstream>


#include "Board.hpp"
#include "Weight.hpp"
#include "Feature.hpp"

namespace crosswalk {

enum class WeightPath {
    SD
};

template<WeightPath path>
class WeightEval {
 public:
    WeightEval() {
        if (path == WeightPath::SD) weightPath = "../weight/SD/";
        loadWeight();
    }
    i64 operator()(const Board &board, CellState color) const {
        i64 res = 0;
        Feature feature;
        auto black = board.getBitBoard(crosswalk::CellState::BLACK);
        auto white = board.getBitBoard(crosswalk::CellState::WHITE);
        feature.update(black, white);

        int stage = board.getTurnCount() / 4;

        res = eval(weight[stage], feature) + 0.5;

        return res;
    }

 private:
    void loadWeight() {
        for (int stage = 0; stage < 15; stage++) {
            double w;
            std::string tag;
            std::stringstream ss;
            ss << "weight";
            if (stage < 10) ss << "0";
            ss << stage;
            std::ifstream ifs(weightPath + ss.str());

            ifs >> tag >> weight[stage].mobility;
            ifs >> tag >> weight[stage].parity;
            for (int i = 0; i < weight[stage].vector2.size(); i++) {
                ifs >> tag >> weight[stage].vector2[i];
            }
            for (int i = 0; i < weight[stage].vector3.size(); i++) {
                ifs >> tag >> weight[stage].vector3[i];
            }
            for (int i = 0; i < weight[stage].vector4.size(); i++) {
                ifs >> tag >> weight[stage].vector4[i];
            }
            for (int i = 0; i < weight[stage].diag4.size(); i++) {
                ifs >> tag >> weight[stage].diag4[i];
            }
            for (int i = 0; i < weight[stage].diag5.size(); i++) {
                ifs >> tag >> weight[stage].diag5[i];
            }
            for (int i = 0; i < weight[stage].diag6.size(); i++) {
                ifs >> tag >> weight[stage].diag6[i];
            }
            for (int i = 0; i < weight[stage].diag7.size(); i++) {
                ifs >> tag >> weight[stage].diag7[i];
            }
            for (int i = 0; i < weight[stage].diag8.size(); i++) {
                ifs >> tag >> weight[stage].diag8[i];
            }
            for (int i = 0; i < weight[stage].edge2X.size(); i++) {
                ifs >> tag >> weight[stage].edge2X[i];
            }
            for (int i = 0; i < weight[stage].corner2x5.size(); i++) {
                ifs >> tag >> weight[stage].corner2x5[i];
            }
            for (int i = 0; i < weight[stage].corner3x3.size(); i++) {
                ifs >> tag >> weight[stage].corner3x3[i];
            }
        }
    }

    double eval(const crosswalk::Weight<double> &weight,
                const crosswalk::Feature &feature) const {
        double ret = 0.0;
        ret += weight.mobility * feature.getMobility();
        ret += weight.parity * feature.getParity();

        ret += weight.diag8[feature.getDiag8(0)];
        ret += weight.diag8[feature.getDiag8(1)];

        for (int i = 0; i < 4; i++) {
            ret += weight.vector2[feature.getVector2(i)];
            ret += weight.vector3[feature.getVector3(i)];
            ret += weight.vector4[feature.getVector4(i)];

            ret += weight.diag4[feature.getDiag4(i)];
            ret += weight.diag5[feature.getDiag5(i)];
            ret += weight.diag6[feature.getDiag6(i)];
            ret += weight.diag7[feature.getDiag7(i)];

            ret += weight.edge2X[feature.getEdge2X(i)];
            ret += weight.corner3x3[feature.getCorner3x3(i)];

            ret += weight.corner2x5[feature.getCorner2x5(i)];
            ret += weight.corner2x5[feature.getCorner2x5(i + 4)];
        }

        return ret;
    }
    std::string weightPath;
    std::array<Weight<double>, 15> weight;
};

}  // crosswalk

#endif  // INCLUDE_CROSSWALK_WEIGHT_EVAL_HPP
