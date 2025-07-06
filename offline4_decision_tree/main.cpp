#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include "process.h"
#include "ID3.h"
#include "heuristic.h"

using namespace std;

void discretizeAdultDataset(Process& process) {
    int n = 5;
    process.discretizeNumericalColumn(0, n);
    process.discretizeNumericalColumn(2, n);
    process.discretizeNumericalColumn(10, n);
    process.discretizeNumericalColumn(11, n);
    process.discretizeNumericalColumn(12, n);
}

void discretizeIrisDataset(Process& process) {
    int n = 3;
    process.discretizeNumericalColumn(0, n);
    process.discretizeNumericalColumn(1, n);
    process.discretizeNumericalColumn(2, n);
    process.discretizeNumericalColumn(3, n);
}

void trainAndTestDataset(string filename, double splitRatio, bool discretize = false,
    HeuristicType heuristicType = HeuristicType::NWIG, int depth = 0,
    bool printTestInfo = true, int repeat = 1, bool generateReport = false, string reportFilename = "") {

    ofstream reportFile;

    if (generateReport && !reportFilename.empty()) {
        reportFile.open(reportFilename + ".csv");

        if (!reportFile.is_open()) {
            cerr << "Error: Unable to open report file." << endl;
            return;
        }
        reportFile << "Discretize,Heuristic,Depth,Number of Nodes,Maximum Depth,Accuracy\n";
    }

    int count = 0;
    while (repeat--) {
        cout << "Starting iteration " << ++count << " for dataset: " << filename << endl;

        cout << "Loading dataset" << endl;
        Process process(filename, splitRatio);

        if (discretize) {
            string str = filename;
            transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });

            if (str.find("iris") != string::npos) {
                discretizeIrisDataset(process);
            }
            else if (str.find("adult") != string::npos) {
                discretizeAdultDataset(process);
            }
            else {
                cout << "Discretization not implemented for this dataset." << endl;
            }
        }

        process.startProcessing();
        vector<AttributeAllValues> attributes = process.getAttributeAllValues();
        vector<TrainingData> trainingData = process.getTrainingData();
        vector<TestData> testData = process.getTestData();
        cout << "Dataset loaded successfully." << endl;

        cout << "Training Decision Tree." << endl;
        Heuristic heuristic(heuristicType); 
        ID3 id3;
        id3.train(trainingData, attributes, heuristic, depth);
        cout << "Training completed." << endl;

        id3.printTree();
        cout << endl;
        cout << "Number of Nodes in the Tree: " << id3.numberOfNodes() << endl;
        cout << "Maximum Depth of the Tree: " << id3.maximumDepth() << endl;
        cout << endl;

        int correctPredictions = 0;
        int totalPredictions = 0;
        for (auto test : testData) {
            Label result = id3.classify(test);
            if (printTestInfo) cout << "Test Data: ";
            for (const auto& attr : test.attributes) {
                if (printTestInfo) cout << attr.name << "=" << attr.value << " ";
            }
            if (printTestInfo) cout << "=> Predicted Label: " << result.name << endl;
            Label actualLabel = process.getLabelForTestData(test);
            if (printTestInfo) cout << "Actual Label: " << actualLabel.name << endl;
            if (result.name == actualLabel.name) {
                if (printTestInfo) cout << "PREDICTION IS CORRECT." << endl;
                correctPredictions++;
            }
            else {
                if (printTestInfo) cout << "PREDICTION IS INCORRECT." << endl;
            }
            totalPredictions++;
            if (printTestInfo) cout << "----------------------------------------" << endl;
        }

        cout << "Total Predictions: " << totalPredictions << endl;
        cout << "Correct Predictions: " << correctPredictions << endl;
        cout << "Incorrect Predictions: " << (totalPredictions - correctPredictions) << endl;
        double accuracy = static_cast<double>(correctPredictions) / totalPredictions * 100.0;
        cout << "Accuracy: " << accuracy << "%" << endl;

        if (generateReport && reportFile.is_open()) {
            string heuristicName;
            switch (heuristicType) {
                case HeuristicType::IG:
                    heuristicName = "IG";
                    break;
                case HeuristicType::IGR:
                    heuristicName = "IGR";
                    break;
                case HeuristicType::NWIG:
                    heuristicName = "NWIG";
                    break;
                default:
                    heuristicName = "Unknown";
                    break;
            }
            int numNodes = id3.numberOfNodes();
            int maxDepth = id3.maximumDepth();

            reportFile << (discretize ? "Yes" : "No") << ","
                       << heuristicName << ","
                       << depth << ","
                       << numNodes << ","
                       << maxDepth << ","
                       << accuracy << "\n";
        }
    }

    if (reportFile.is_open()) {
        reportFile.close();
    }
}

int main()
{
    // discretize
    // heuristic type
    // depth
    string filename = "Datasets/adult.data";
    double splitRatio = 0.8;
    bool discretize = true;

    HeuristicType heuristicType = HeuristicType::IGR;
    int depth = 0; // 0 means no limit on depth

    bool printTestInfo = false;
    int repeat = 3;
    bool generateReport = true;
    string reportFilename = "my_report.csv";

    trainAndTestDataset(filename, splitRatio, discretize, heuristicType, depth, printTestInfo, repeat, generateReport, reportFilename);

    return 0;
}
