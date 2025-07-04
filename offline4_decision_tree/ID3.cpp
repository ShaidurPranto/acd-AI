#include<iostream>
#include <map>
#include <cmath>
#include "util.h"
#include "heuristic.h"
#include "ID3.h"

using namespace std;


void ID3::deleteTree(Node* node) {
    if (node) {
        for (auto child : node->children) {
            deleteTree(child);
        }
        delete node;
    }
}

bool ID3::allSameLabel(vector<TrainingData> trainingData) {
    if (trainingData.empty()) return true;
    Label firstLabel = trainingData[0].label;
    for (auto data : trainingData) {
        if (data.label.name != firstLabel.name) {
            return false;
        }
    }
    return true;
}

Label ID3::getCommonLabel(vector<TrainingData> trainingData) {
    map<Label, int> labelCount;
    for (auto data : trainingData) {
        labelCount[data.label]++;
    }
    Label mostCommonLabel;
    int maxCount = 0;
    for (auto pair : labelCount) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mostCommonLabel = pair.first;
        }
    }
    return mostCommonLabel;
}

AttributeAllValues ID3::getBestAttribute(vector<TrainingData> trainingData, vector<AttributeAllValues> attributes, Heuristic heuristic) {
    double bestScore = heuristic.evaluate(trainingData, attributes[0]);
    AttributeAllValues bestAttribute = attributes[0];

    for (auto attribute : attributes) {
        double score = heuristic.evaluate(trainingData, attribute);
        if (score > bestScore) {
            bestScore = score;
            bestAttribute = attribute;
        }
    }
    return bestAttribute;
}

vector<TrainingData> ID3::filterDataByAttributeValue(vector<TrainingData> data, AttributeValue attributeValue) {
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

vector<AttributeAllValues> ID3::getRemainingAttributes(vector<AttributeAllValues> attributes, AttributeAllValues bestAttribute) {
    vector<AttributeAllValues> remainingAttributes;
    for (auto attr : attributes) {
        if (attr.name != bestAttribute.name) {
            remainingAttributes.push_back(attr);
        }
    }
    return remainingAttributes;
}

void ID3::buildTree(Node * parent,vector<TrainingData> trainingData, vector<AttributeAllValues> attributes, Heuristic heuristic, int depth, bool isDepth) {
    AttributeAllValues bestAttribute = getBestAttribute(trainingData, attributes, heuristic);
    vector<AttributeAllValues> remainingAttributes = getRemainingAttributes(attributes, bestAttribute);
    // cout << "Best attribute: " << bestAttribute.name << endl;

    for (auto value : bestAttribute.values) {
        Node* childNode = new Node();
        childNode->parent = parent;
        parent->children.push_back(childNode);

        childNode->attribute.name = bestAttribute.name;
        childNode->attribute.value = value;

        // cout << "attribute : " << childNode->attribute.name << ", attribute value : " << childNode->attribute.value << endl;

        // filter training data for this attribute value
        vector<TrainingData> filteredData = filterDataByAttributeValue(trainingData, {bestAttribute.name, value});

        // if no data left, assign common label
        if (filteredData.empty()) {
            childNode->label = getCommonLabel(trainingData);
            // cout << "LEAF NODE: no more example with this attribute and value , assigning label : " << childNode->label.name << endl;
        } else if(allSameLabel(filteredData)) {
            childNode->label = filteredData[0].label;
            // cout << "LEAF NODE: all examples have same label: " << childNode->label.name << endl;
        } else if (remainingAttributes.empty()) {
            childNode->label = getCommonLabel(filteredData);
            // cout << "LEAF NODE: no more attributes left, assigning common label: " << childNode->label.name << endl;
        } else if (isDepth && depth <= 1) {
            childNode->label = getCommonLabel(filteredData);
            // cout << "LEAF NODE: depth limit reached, assigning common label: " << childNode->label.name << endl;
        } else {
            buildTree(childNode,filteredData,remainingAttributes,heuristic,depth - 1,isDepth);
        }
    }
}

Label ID3::classifyHelper(Node* node, TestData testData) {
    if (node->children.empty()) {
        return {node->label}; 
    }

    for (auto child : node->children) {
        for (auto attr : testData.attributes) {
            if (attr.name == child->attribute.name && attr.value == child->attribute.value) {
                return classifyHelper(child, testData);
            }
        }
    }
    return {}; 
}

void ID3::printTreeHelper(Node* node, string indent, bool isLast) {
    if (node == NULL) return;

    cout << indent;
    if (isLast) {
        cout << "└── ";
        indent += "    ";
    } else {
        cout << "├── ";
        indent += "│   ";
    }

    if (node->children.empty()) {
        cout << "Attribute: " << node->attribute.name << " = " << node->attribute.value << ", Label: " << node->label.name << endl;
    } else {
        cout << "Attribute: " << node->attribute.name << " = " << node->attribute.value << endl;
    }

    for (size_t i = 0; i < node->children.size(); ++i) {
        printTreeHelper(node->children[i], indent, i == node->children.size() - 1);
    }
}




ID3::ID3() {
    root = NULL;
}

ID3::~ID3() {
    deleteTree(root);
}

void ID3::train(vector<TrainingData> trainingData, vector<AttributeAllValues> attributes, Heuristic heuristic, int depth) {
    this->root = new Node();
    if(depth == 0) {
        buildTree(this->root,trainingData, attributes, heuristic, depth, false);
    } else {
        buildTree(this->root,trainingData, attributes, heuristic, depth, true);
    }
}

Label ID3::classify(TestData testData) {
    if (root == NULL) {
        throw runtime_error("Model is not trained yet.");
    }
    return classifyHelper(root, testData);
}

void ID3::printTree() {
    if (root == NULL) {
        cout << "Tree is empty." << endl;
        return;
    }
    cout << "Decision Tree:" << endl;
    for (size_t i = 0; i < root->children.size(); ++i) {
        printTreeHelper(root->children[i], "", i == root->children.size() - 1);
    }
}
