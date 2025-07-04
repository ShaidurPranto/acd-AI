#include<iostream>
#include "process.h"
#include "ID3.h"
#include "heuristic.h"

using namespace std;


int main()
{
    string filename = "Datasets/adult.data";
    double splitRatio = 0.8;

    Process process(filename, splitRatio);
    process.trimFirstColumnFromFile();
    process.startProcessing();
    vector<AttributeAllValues> attributes = process.getAttributeAllValues();
    vector<TrainingData> trainingData = process.getTrainingData();
    vector<TestData> testData = process.getTestData();
    cout << "Dataset loaded successfully." << endl;

    cout << "Training Decision Tree using ID3 algorithm..." << endl;
    Heuristic heuristic(HeuristicType::IG);
    ID3 id3;
    id3.train(trainingData, attributes, heuristic, 5);
    cout << "Training completed." << endl;

    id3.printTree();
    cout << endl << endl;

    for (auto test : testData) {
        Label result = id3.classify(test);
        cout << "Test Data: ";
        for (const auto& attr : test.attributes) {
            cout << attr.name << "=" << attr.value << " ";
        }
        cout << "=> Predicted Label: " << result.name << endl;
        Label actualLabel = process.getLabelForTestData(test);
        cout << "Actual Label: " << actualLabel.name << endl;
        if (result.name == actualLabel.name) {
            cout << "PREDICTION IS CORRECT." << endl;
        } else {
            cout << "PREDICTION IS INCORRECT." << endl;
        }
        cout << "----------------------------------------" << endl;
    }
    return 0;
}