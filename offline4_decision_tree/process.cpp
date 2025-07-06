#include "process.h"
#include <cfloat>

vector<string> Process::splitCSV(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (getline(ss, token, ',')) {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }

    return tokens;
}
bool Process::areAttributesEqual(const vector<AttributeValue>& a, const vector<AttributeValue>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].name != b[i].name || a[i].value != b[i].value) {
            return false;
        }
    }
    return true;
}
void Process::readDataFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return;
    }

    // read header line
    string line;
    if (getline(file, line)) {
        headers = splitCSV(line);
    }
    if (headers.size() < 2) {
        cerr << "No attributes found (need at least 2 columns)" << endl;
        return;
    }

    // read data lines
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> tokens = splitCSV(line);
        if (tokens.size() != headers.size()) {
            cerr << "Skipping line with incorrect number of columns: " << line << endl;
            continue;
        }

        TrainingData data;

        // skip last column
        for (int i = 0; i < headers.size() - 1; ++i) {
            AttributeValue av;
            av.name = headers[i];
            av.value = tokens[i];
            data.attributes.push_back(av);
        }

        // Process label (last column)
        data.label.name = tokens[tokens.size() - 1];
        allData.push_back(data);
    }
    file.close();
}
void Process::collectAttributes() {
    int attrIndex = 0;
    for (int i = 0; i < headers.size() - 1; ++i) {
        AttributeAllValues aav;
        aav.name = headers[i];

        for (auto d : allData) {
            if (attrIndex < d.attributes.size()) {
                string v = d.attributes[attrIndex].value;
                if (find(aav.values.begin(), aav.values.end(), v) == aav.values.end()) {
                    aav.values.push_back(v);
                }
            }
        }

        attributes.push_back(aav);
        attrIndex++;
    }
}
void Process::readInitialDataFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        initial.push_back(line);
    }
    file.close();
}
void Process::writeInitialDataToFile() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file for writing: " << filename << endl;
        return;
    }

    for (const auto& line : initial) {
        file << line << endl;
    }
    file.close();
}

Process::Process(string filename, double splitRatio) {
    this->filename = filename;
    this->splitRatio = splitRatio;
    readInitialDataFromFile();
}
Process::~Process() {
    writeInitialDataToFile();
}
void Process::trimFirstColumnFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return;
    }

    string line;
    vector<string> lines;

    // read all lines
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> tokens = splitCSV(line);
        if (tokens.size() < 2) continue; 
        lines.push_back(line);
    }
    file.close();

    // write back without first column
    ofstream outFile(filename);
    for (const auto& l : lines) {
        vector<string> tokens = splitCSV(l);
        for (size_t i = 1; i < tokens.size(); ++i) { 
            outFile << tokens[i];
            if (i < tokens.size() - 1) outFile << ",";
        }
        outFile << endl;
    }
}
void Process::addHeaderRowInFile() {
}
void Process::startProcessing() {
    // read data from file
    readDataFromFile();
    if (allData.empty()) {
        cerr << "No valid data found in file" << endl;
        return;
    }

    // collect attributes
    collectAttributes();

    // shuffle and split data
    random_device rd;
    mt19937 g(rd());
    shuffle(allData.begin(), allData.end(), g);

    size_t trainSize = static_cast<size_t>(splitRatio * allData.size());
    if (trainSize == 0) {
        cerr << "Training size is 0. Check your split ratio." << endl;
        return;
    }

    // assign training data
    trainingData.assign(allData.begin(), allData.begin() + trainSize);

    // assign test data
    for (auto it = allData.begin() + trainSize; it != allData.end(); ++it) {
        TestData td;
        td.attributes = it->attributes;
        testData.push_back(td);
    }

    cout << "Processing complete:" << endl;
    cout << "Total samples: " << allData.size() << endl;
    cout << "Training samples: " << trainingData.size() << endl;
    cout << "Test samples: " << testData.size() << endl;
    cout << "Attributes: " << attributes.size() << endl;

    cout << "Attribute Information:" << endl;
    for (int i = 0; i < attributes.size(); ++i) {
        cout << "Attribute " << i << ": " << attributes[i].name << ", Number of Possible Values: " << attributes[i].values.size() << endl;
        // cout << "  Possible values: ";
        // for (const auto& val : attributes[i].values) {
        //     cout << val << ", ";
        // }
        // cout << endl;
    }
}

vector<AttributeAllValues> Process::getAttributeAllValues() {
    return attributes;
}
vector<TrainingData> Process::getTrainingData() {
    return trainingData;
}
vector<TestData> Process::getTestData() {
    return testData;
}
Label Process::getLabelForTestData(TestData testData) {
    for (const auto& data : allData) {
        if (areAttributesEqual(data.attributes, testData.attributes)) {
            return data.label;
        }
    }
    Label defaultLabel;
    return defaultLabel;
}
void Process::discretizeNumericalColumn(int columnIndex, int numClasses) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return;
    }

    vector<vector<string>> rows;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> tokens = splitCSV(line);
        if (tokens.size() <= columnIndex) {
            cerr << "Line skipped due to insufficient columns: " << line << endl;
            continue;
        }
        rows.push_back(tokens);
    }
    file.close();

    double minVal = DBL_MAX;
    double maxVal = -DBL_MAX;

    for (size_t i = 1; i < rows.size(); ++i) {
        try {
            double val = stod(rows[i][columnIndex]);
            minVal = min(minVal, val);
            maxVal = max(maxVal, val);
        } catch (...) {
            cerr << "Skipping non-numeric value: " << rows[i][columnIndex] << endl;
        }
    }

    if (minVal >= maxVal) {
        cerr << "Invalid range for discretization. min: " << minVal << ", max: " << maxVal << endl;
        return;
    }

    double interval = (maxVal - minVal) / numClasses;

    // Replace values with class labels
    for (size_t i = 1; i < rows.size(); ++i) {
        try {
            double val = stod(rows[i][columnIndex]);
            int classIndex = static_cast<int>((val - minVal) / interval);
            if (classIndex == numClasses) classIndex--; // edge case for max value
            rows[i][columnIndex] = "Class" + to_string(classIndex + 1);
        } catch (...) {
            cerr << "Skipping non-numeric value during class assignment: " << rows[i][columnIndex] << endl;
        }
    }

    // Write back to file
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Could not open file for writing: " << filename << endl;
        return;
    }

    for (const auto& tokens : rows) {
        for (size_t i = 0; i < tokens.size(); ++i) {
            outFile << tokens[i];
            if (i < tokens.size() - 1) outFile << ",";
        }
        outFile << endl;
    }
    outFile.close();

    cout << "Discretization complete for column " << columnIndex << " into " << numClasses << " classes." << endl;
}
