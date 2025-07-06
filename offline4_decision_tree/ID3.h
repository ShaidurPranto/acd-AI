#ifndef ID3_H
#define ID3_H

#include<iostream>
#include "util.h"
#include "heuristic.h"

using namespace std;

class ID3 {
private:
    Node* root;

    void deleteTree(Node* node);
    bool allSameLabel(vector<TrainingData> trainingData);
    Label getCommonLabel(vector<TrainingData> trainingData);
    AttributeAllValues getBestAttribute(vector<TrainingData> &trainingData, vector<AttributeAllValues> &attributes, Heuristic &heuristic);
    vector<TrainingData> filterDataByAttributeValue(vector<TrainingData> data, AttributeValue attributeValue);
    vector<AttributeAllValues> getRemainingAttributes(vector<AttributeAllValues> attributes, AttributeAllValues bestAttribute);
    void buildTree(Node * parent,vector<TrainingData> trainingData, vector<AttributeAllValues> attributes, Heuristic heuristic, int depth, bool isDepth = false);
    Label classifyHelper(Node* node, TestData testData);
    void printTreeHelper(Node* node, string indent, bool isLast);
    int numberOfNodesHelper(Node* node);
    int maximumDepthHelper(Node* node, int currentDepth);
public:
    ID3();
    ~ID3();
    void train(vector<TrainingData> trainingData, vector<AttributeAllValues> attributes, Heuristic heuristic, int depth);
    Label classify(TestData testData);
    void printTree();
    int numberOfNodes();
    int maximumDepth();
};

#endif