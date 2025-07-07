#ifndef PROCESS_H
#define PROCESS_H

#include "2105177_util.h"
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

    vector<string> initial;
    vector<string> headers;
    vector<AttributeAllValues> attributes;
    vector<TrainingData> allData;
    vector<TrainingData> trainingData;
    vector<TestData> testData;
    
    vector<string> splitCSV(const string& line);
    bool areAttributesEqual(const vector<AttributeValue>& a, const vector<AttributeValue>& b);
    void readDataFromFile();
    void collectAttributes();
    void readInitialDataFromFile();
    void writeInitialDataToFile();
public:
    Process(string filename, double splitRatio);
    ~Process();
    void trimFirstColumnFromFile();
    void addHeaderRowInFile();
    void startProcessing();
    vector<AttributeAllValues> getAttributeAllValues();
    vector<TrainingData> getTrainingData();
    vector<TestData> getTestData();
    Label getLabelForTestData(TestData testData);
    void discretizeNumericalColumn(int columnIndex, int numClasses);
};

#endif