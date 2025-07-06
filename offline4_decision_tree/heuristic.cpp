#include<iostream>
#include <map>
#include <cmath>
#include "util.h"
#include "heuristic.h"

using namespace std;

double Heuristic::entropy(vector<TrainingData> data) {
    if (data.empty()) return 0.0;

    map<Label, int> classCounts;
    for (auto sample : data) {
        classCounts[sample.label]++;
    }

    double entropy = 0.0;
    int totalSamples = data.size();

    for (auto pair : classCounts) {
        double probability = (double)pair.second / totalSamples;
        if (probability > 0) {
            entropy =  entropy - probability * log2(probability);
        }
    }

    return entropy;
}

vector<TrainingData> Heuristic::filterDataByAttributesValue(vector<TrainingData> data, AttributeValue attributeValue) {
    vector<TrainingData> filteredData;
    for (auto sample : data) {
        for (auto attr : sample.attributes) {
            if (attr.name == attributeValue.name && attr.value == attributeValue.value) {
                filteredData.push_back(sample);
                break;
            }
        }
    }
    return filteredData;
}

double Heuristic::informationGain(vector<TrainingData> data, AttributeAllValues attribute) {
    double totalEntropy = entropy(data);
    int totalSamples = data.size();

    for (auto value : attribute.values) {
        AttributeValue attrValue{attribute.name, value};
        vector<TrainingData> filteredData = filterDataByAttributesValue(data, attrValue);
        double subsetEntropy = entropy(filteredData);
        totalEntropy = totalEntropy - ((double)filteredData.size() / totalSamples) * subsetEntropy;
    }
    return totalEntropy;
}

double Heuristic::informationGainRatio(vector<TrainingData> data, AttributeAllValues attribute) {
    double intrinsicValue = 0.0;
    int totalSamples = data.size();
    for (auto value : attribute.values) {
        AttributeValue attrValue{attribute.name, value};
        vector<TrainingData> filteredData = filterDataByAttributesValue(data, attrValue);
        double subsetSize = (double)filteredData.size() / totalSamples;
        if (subsetSize > 0) {
            intrinsicValue = intrinsicValue - subsetSize * log2(subsetSize);
        }
    }
    return intrinsicValue == 0 ? 0 : informationGain(data, attribute) / intrinsicValue;
}

double Heuristic::normalizedWeightedInformationGain(vector<TrainingData> data, AttributeAllValues attribute) {
    double informationGainValue = informationGain(data, attribute);
    int k = attribute.values.size();
    int sampleSize = data.size();
    return (informationGainValue / log2(k+1)) * (1 - (k - 1) / sampleSize);
}

Heuristic::Heuristic(HeuristicType type = HeuristicType::IG) {
    this->type = type;
}

double Heuristic::evaluate(vector<TrainingData> data, AttributeAllValues attribute) {
    switch (type) {
    case HeuristicType::IG:
        return informationGain(data, attribute);
    case HeuristicType::IGR:
        return informationGainRatio(data, attribute);
    case HeuristicType::NWIG:
        return normalizedWeightedInformationGain(data, attribute);
    }
    return 0.0;
}