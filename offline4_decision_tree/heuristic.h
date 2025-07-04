#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <iostream>
#include "util.h"

enum class HeuristicType {
    IG,
    IGR,
    NWIG
};

class Heuristic {
    HeuristicType type;

    double entropy(vector<TrainingData> data);
    vector<TrainingData> filterDataByAttributesValue(vector<TrainingData> data, AttributeValue attributeValue);
    double informationGain(vector<TrainingData> data, AttributeAllValues attribute);
    double informationGainRatio(vector<TrainingData> data, AttributeAllValues attribute);
    double normalizedWeightedInformationGain(vector<TrainingData> data, AttributeAllValues attribute);
public:
    Heuristic(HeuristicType type);
    double evaluate(vector<TrainingData> data, AttributeAllValues attribute);
};



#endif