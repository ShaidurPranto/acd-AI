#include<iostream>
#include "process.h"
#include "ID3.h"
#include "heuristic.h"

using namespace std;

void discretizeAdultDataset(Process& process) {
    process.discretizeNumericalColumn(0, 3);  
    process.discretizeNumericalColumn(2, 3);  
    process.discretizeNumericalColumn(10, 3); 
    process.discretizeNumericalColumn(11, 3); 
    process.discretizeNumericalColumn(12, 3); 
}
void discretizeIrisDataset(Process& process) {
    process.discretizeNumericalColumn(0, 3);  
    process.discretizeNumericalColumn(1, 3);  
    process.discretizeNumericalColumn(2, 3); 
    process.discretizeNumericalColumn(3, 3); 
}


int main()
{
    string filename = "Datasets/adult.data";
    double splitRatio = 0.8;

    Process process(filename, splitRatio);
    discretizeAdultDataset(process);

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

    int correctPredictions = 0;
    int totalPredictions = 0;
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
            correctPredictions++;
        } else {
            cout << "PREDICTION IS INCORRECT." << endl;
        }
        totalPredictions++;
        cout << "----------------------------------------" << endl;
    }


    cout << "Total Predictions: " << totalPredictions << endl;
    cout << "Correct Predictions: " << correctPredictions << endl;
    cout << "Incorrect Predictions: " << (totalPredictions - correctPredictions) << endl;
    double accuracy = static_cast<double>(correctPredictions) / totalPredictions * 100.0;
    cout << "Accuracy: " << accuracy << "%" << endl;

    return 0;
}