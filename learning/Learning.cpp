/*
 * 標準入力から譜面を読み込み
 * ステージごとに各ファイルに重みを書き出す
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <random>

#include "../src/Board.hpp"
#include "../src/Config.hpp"
#include "../src/Utils.hpp"
#include "../src/Feature.hpp"
#include "../src/Weight.hpp"

class Learning {
 public:
    void operator()(int iteration) {
        weight = {};
        diff = {};
        setOccurredCount();

        featureCount = features.size();

        for (int i = 0; i < iteration; i++) {
            std::cerr << "iteration: " << i << std::endl;
            weight = update(weight).first;
        }
        std::stringstream ss;
        ss << "weight";
        if(stage < 10) ss << "0";
        ss << stage;
        writeWeight(weight, ss.str());
    }

    void setFeatures(
        const std::vector<std::pair<crosswalk::Feature, int>> &features) {
        this->features = features;
    }

    void setStage(int stage) { this->stage = stage; }

 private:
    void writeWeight(const crosswalk::Weight<double> &w,
                     const std::string &filename) {
        std::ofstream ofs(filename.c_str());
        ofs << "mobility " << w.mobility << std::endl;
        ofs << "parity " << w.parity << std::endl;

        for (int i = 0; i < w.vector2.size(); i++) {
            ofs << "vector2_" << i << " " << w.vector2[i] << std::endl;
        }
        for (int i = 0; i < w.vector3.size(); i++) {
            ofs << "vector3_" << i << " " << w.vector3[i] << std::endl;
        }
        for (int i = 0; i < w.vector4.size(); i++) {
            ofs << "vector4_" << i << " " << w.vector4[i] << std::endl;
        }
        for (int i = 0; i < w.diag4.size(); i++) {
            ofs << "diag4_" << i << " " << w.diag4[i] << std::endl;
        }
        for (int i = 0; i < w.diag5.size(); i++) {
            ofs << "diag5_" << i << " " << w.diag5[i] << std::endl;
        }
        for (int i = 0; i < w.diag6.size(); i++) {
            ofs << "diag6_" << i << " " << w.diag6[i] << std::endl;
        }
        for (int i = 0; i < w.diag7.size(); i++) {
            ofs << "diag7_" << i << " " << w.diag7[i] << std::endl;
        }
        for (int i = 0; i < w.diag8.size(); i++) {
            ofs << "diag8_" << i << " " << w.diag8[i] << std::endl;
        }
        for (int i = 0; i < w.edge2X.size(); i++) {
            ofs << "e2x_" << i << " " << w.edge2X[i] << std::endl;
        }
        for (int i = 0; i < w.corner2x5.size(); i++) {
            ofs << "cor25_" << i << " " << w.corner2x5[i] << std::endl;
        }
        for (int i = 0; i < w.corner3x3.size(); i++) {
            ofs << "cor33_" << i << " " << w.corner3x3[i] << std::endl;
        }
    }
    void setOccurredCount() {
        occurredCount = {};
        for (const auto &e : features) {
            const auto &feature = e.first;
            occurredCount.parity++;
            occurredCount.mobility++;

            occurredCount.diag8[feature.getDiag8(0)]++;
            occurredCount.diag8[feature.getDiag8(1)]++;

            for (int i = 0; i < 4; i++) {
                occurredCount.vector2[feature.getVector2(i)]++;
                occurredCount.vector3[feature.getVector3(i)]++;
                occurredCount.vector4[feature.getVector4(i)]++;

                occurredCount.diag4[feature.getDiag4(i)]++;
                occurredCount.diag5[feature.getDiag5(i)]++;
                occurredCount.diag6[feature.getDiag6(i)]++;
                occurredCount.diag7[feature.getDiag7(i)]++;

                occurredCount.edge2X[feature.getEdge2X(i)]++;
                occurredCount.corner3x3[feature.getCorner3x3(i)]++;

                occurredCount.corner2x5[feature.getCorner2x5(i)]++;
                occurredCount.corner2x5[feature.getCorner2x5(i + 4)]++;
            }
        }
    }
    // 評価値の値を計算する
    double eval(const crosswalk::Weight<double> &weight,
                const crosswalk::Feature &feature) {
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

    // 更新した重みと二乗誤差平均を返す
    std::pair<crosswalk::Weight<double>, double> update(
        const crosswalk::Weight<double> &weight) {
        double total = 0.0;
        crosswalk::Weight<double> diff;

        for (const auto &e : features) {
            const auto &feature = e.first;
            int lastScore = e.second;
            double error = lastScore - eval(weight, feature);
            total += error * error;

            diff.parity += error * feature.getParity();
            diff.mobility += error * feature.getMobility();
            diff.diag8[feature.getDiag8(0)] += error;
            diff.diag8[feature.getDiag8(1)] += error;

            for (int i = 0; i < 4; i++) {
                diff.vector2[feature.getVector2(i)] += error;
                diff.vector3[feature.getVector3(i)] += error;
                diff.vector4[feature.getVector4(i)] += error;

                diff.diag4[feature.getDiag4(i)] += error;
                diff.diag5[feature.getDiag5(i)] += error;
                diff.diag6[feature.getDiag6(i)] += error;
                diff.diag7[feature.getDiag7(i)] += error;

                diff.edge2X[feature.getEdge2X(i)] += error;
                diff.corner3x3[feature.getCorner3x3(i)] += error;
                diff.corner2x5[feature.getCorner2x5(i)] += error;
                diff.corner2x5[feature.getCorner2x5(i + 4)] += error;
            }
        }
        return std::make_pair(updateWeight(weight, diff), total);
    }

    crosswalk::Weight<double> updateWeight(const crosswalk::Weight<double> &w,
                                           const crosswalk::Weight<double> &d) {
        crosswalk::Weight<double> nw;
        auto diff = nomalize(d);

        nw.parity = w.parity + diff.parity;
        nw.mobility = w.mobility + diff.mobility;

        for (int i = 0; i < nw.vector2.size(); i++) {
            nw.vector2[i] = w.vector2[i] + diff.vector2[i];
            nw.vector3[i] = w.vector3[i] + diff.vector3[i];
            nw.vector4[i] = w.vector4[i] + diff.vector4[i];
            nw.diag8[i] = w.diag8[i] + diff.diag8[i];
        }

        for (int i = 0; i < nw.diag7.size(); i++) {
            if (i < nw.diag4.size()) nw.diag4[i] = w.diag4[i] + diff.diag4[i];
            if (i < nw.diag5.size()) nw.diag5[i] = w.diag5[i] + diff.diag5[i];
            if (i < nw.diag6.size()) nw.diag6[i] = w.diag6[i] + diff.diag6[i];
            nw.diag7[i] = w.diag7[i] + diff.diag7[i];
        }

        for (int i = 0; i < nw.corner2x5.size(); i++) {
            nw.corner2x5[i] = w.corner2x5[i] + diff.corner2x5[i];
            nw.edge2X[i] = w.edge2X[i] + diff.edge2X[i];
            if (i < nw.corner3x3.size())
                nw.corner3x3[i] = w.corner3x3[i] + diff.corner3x3[i];
        }

        return nw;
    }

    crosswalk::Weight<double> nomalize(
        const crosswalk::Weight<double> &weight) {
        crosswalk::Weight<double> nw;

        nw.parity = nomalize(weight.parity, occurredCount.parity, beta);
        nw.mobility = nomalize(weight.mobility, occurredCount.mobility, beta);

        for (int i = 0; i < nw.vector2.size(); i++) {
            nw.vector2[i] =
                nomalize(weight.vector2[i], occurredCount.vector2[i], beta);
            nw.vector3[i] =
                nomalize(weight.vector3[i], occurredCount.vector3[i], beta);
            nw.vector4[i] =
                nomalize(weight.vector4[i], occurredCount.vector4[i], beta);
            nw.diag8[i] =
                nomalize(weight.diag8[i], occurredCount.diag8[i], beta);
        }

        for (int i = 0; i < nw.diag7.size(); i++) {
            if (i < nw.diag4.size())
                nw.diag4[i] =
                    nomalize(weight.diag4[i], occurredCount.diag4[i], beta);
            if (i < nw.diag5.size())
                nw.diag5[i] =
                    nomalize(weight.diag5[i], occurredCount.diag5[i], beta);
            if (i < nw.diag6.size())
                nw.diag6[i] =
                    nomalize(weight.diag6[i], occurredCount.diag6[i], beta);
            nw.diag7[i] =
                nomalize(weight.diag7[i], occurredCount.diag7[i], beta);
        }

        for (int i = 0; i < nw.corner2x5.size(); i++) {
            nw.corner2x5[i] =
                nomalize(weight.corner2x5[i], occurredCount.corner2x5[i], beta);
            nw.edge2X[i] =
                nomalize(weight.edge2X[i], occurredCount.edge2X[i], beta);
            if (i < nw.corner3x3.size())
                nw.corner3x3[i] = nomalize(weight.corner3x3[i],
                                           occurredCount.corner3x3[i], beta);
        }

        return nw;
    }

    double nomalize(double diff, int occurredCount, double beta) {
        if (occurredCount == 0) return 0.0;
        return 2.0 * std::min(1.0 / occurredCount, 1.0 / 100.0) * diff / featureCount;
    }
    // 特徴量と最終スコア
    std::vector<std::pair<crosswalk::Feature, int>> features;
    // 各パターンの重み
    crosswalk::Weight<double> weight;
    // 各反復ごとの差分
    crosswalk::Weight<double> diff;
    // 各パターンの発生回数
    crosswalk::Weight<int> occurredCount;
    // 学習率
    double beta = 1.0;
    int featureCount;
    int stage;
};

constexpr int MAX_FEATURE_COUNT = 1000000;
using Features =
    std::array<std::vector<std::pair<crosswalk::Feature, int>>, 15>;
void addFeatures(int lastScore, const crosswalk::Cells &cells,
                 Features &features) {
    crosswalk::Board board;
    crosswalk::CellState color = crosswalk::CellState::BLACK;
    crosswalk::Feature feature;
    for (const auto &cell : cells) {
        if (cell.vaild()) {
            board.putStone(color, cell);
            auto black = board.getBitBoard(crosswalk::CellState::BLACK);
            auto white = board.getBitBoard(crosswalk::CellState::WHITE);

            feature.update(black, white);
            int turn = board.getTurnCount();
            if (features[turn / 4].size() < MAX_FEATURE_COUNT)
                features[turn / 4].emplace_back(feature, lastScore);
            if (turn / 4 - 1 >= 0 && turn % 4 == 0) {
                if (features[turn / 4 - 1].size() < MAX_FEATURE_COUNT)
                    features[turn / 4 - 1].emplace_back(feature, lastScore);
            }
            if (turn / 4 + 1 < 15 && turn % 4 == 3) {
                if (features[turn / 4 + 1].size() < MAX_FEATURE_COUNT)
                    features[turn / 4 + 1].emplace_back(feature, lastScore);
            }
        }
        color = crosswalk::switchCellState(color);
    }
}

int main() {
    std::string line;
    Features features;
    while (std::getline(std::cin, line)) {
        int lastScore = (line[1] - '0') * 10 + (line[2] - '0');
        if (line[0] == '-') lastScore = -lastScore;
        crosswalk::Cells cells;
        for (int i = 3; i < line.size(); i += 2) {
            cells.emplace_back(line.substr(i, 2));
        }
        addFeatures(lastScore, cells, features);
    }

    Learning l;
    for (int stage = 0; stage < 15; stage++) {
        std::cerr << "learning stage: " << stage << std::endl;
        l.setStage(stage);
        l.setFeatures(features[stage]);
        l(500);
    }

    return 0;
}
