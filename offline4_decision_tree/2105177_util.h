#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct Attribute {
    string name;

    bool operator<(const Attribute& other) const {
        return name < other.name;
    }
};

struct AttributeValue {
    string name;
    string value;

    bool operator<(const AttributeValue& other) const {
        if (name != other.name) return name < other.name;
        return value < other.value;
    }
};

struct AttributeAllValues {
    string name;
    vector<string> values;

    bool operator<(const AttributeAllValues& other) const {
        if (name != other.name) return name < other.name;
        return values < other.values;
    }
};

struct Label {
    string name;

    bool operator<(const Label& other) const {
        return name < other.name;
    }
};

struct TrainingData {
    vector<AttributeValue> attributes;
    Label label;
};

struct TestData {
    vector<AttributeValue> attributes;
};

struct Node {
    Node* parent = NULL;
    vector<Node*> children;
    AttributeValue attribute;
    Label label;
};

#endif
