#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include "2105177_process.h"
#include "2105177_ID3.h"
#include "2105177_heuristic.h"

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
        cout << "Starting iteration " << ++count << " for dataset: " << filename << endl << endl;;

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
        cout << endl;

        cout << "Training Decision Tree." << endl;
        Heuristic heuristic(heuristicType); 
        ID3 id3;
        id3.train(trainingData, attributes, heuristic, depth);
        cout << "Training completed." << endl;

        // id3.printTree();
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
HeuristicType getHeuristicTypeFromString(const string& str) {
    if (str == "IG" || str == "ig") return HeuristicType::IG;
    else if (str == "IGR" || str == "igr") return HeuristicType::IGR;
    else if (str == "NWIG" || str == "nwig") return HeuristicType::NWIG;
    else {
        cerr << "Invalid heuristic type. Defaulting to IG." << endl;
        return HeuristicType::IG;
    }
}

// g++ 2105177_main.cpp 2105177_process.cpp 2105177_heuristic.cpp 2105177_ID3.cpp -o out
// g++ -Ofast -march=native 2105177_main.cpp 2105177_process.cpp 2105177_heuristic.cpp 2105177_ID3.cpp -o out
// ./out IG 3

int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Usage: " << argv[0] << " ./decision_tree <criterion> <maxDepth>" << endl;
        return 1;
    }
    string filename = "Datasets/Iris.csv"; 
    double splitRatio = 0.8;
    bool discretize = false;

    HeuristicType heuristicType = getHeuristicTypeFromString(argv[1]);
    int depth = atoi(argv[2]);

    bool printTestInfo = false;
    int repeat = 1;
    bool generateReport = false;
    string reportFilename = "my_report";

    trainAndTestDataset(filename, splitRatio, discretize, heuristicType, depth, printTestInfo, repeat, generateReport, reportFilename);

    return 0;
}
