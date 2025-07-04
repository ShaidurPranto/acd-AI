#ifndef PROCESS_H
#define PROCESS_H

#include "util.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iostream>

using namespace std;

class Process {
private:
    string filename;
    double splitRatio;

    vector<string> headers;
    vector<AttributeAllValues> attributes;
    vector<TrainingData> allData;
    vector<TrainingData> trainingData;
    vector<TestData> testData;
    
    vector<string> splitCSV(const string& line);
    bool areAttributesEqual(const vector<AttributeValue>& a, const vector<AttributeValue>& b);
    void readDataFromFile();
    void collectAttributes();
public:
    Process(string filename, double splitRatio);
    void trimFirstColumnFromFile();
    void addHeaderRowInFile();
    void startProcessing();
    vector<AttributeAllValues> getAttributeAllValues() const { return attributes; }
    vector<TrainingData> getTrainingData() const { return trainingData; }
    vector<TestData> getTestData() const { return testData; }
    Label getLabelForTestData(TestData testData);
};

#endif