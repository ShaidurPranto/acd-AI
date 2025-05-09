#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <vector>
#include <random>

#include "graph.hpp"

using namespace std;

Edge get_max_weight_edge(Graph g){
    int n = g.weights.size();
    Edge edge;
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            if(edge.weight < g.weights[i][j]){
                edge.vertex_a = i;
                edge.vertex_b = j;
                edge.weight = g.weights[i][j];
            }
        }
    }
    return edge;
}

// returns sum of weights of edges between vertex and group
int value_for_group(Graph g,int vertex, vector<int> group){
    int value = 0;
    for(int i=0;i<group.size();i++){
        value += g.weights[vertex][group[i]];
    }
    return value;
}

// returns sum of weights of a cut
int weight_of_cut(Graph g, Cut cut){
    int weight = 0;
    for(int i=0;i<cut.x.size();i++){
        for(int j=0;j<cut.y.size();j++){
            weight += g.weights[cut.x[i]][cut.y[j]];
        }
    }
    return weight;
}

// returns true if vertex is in group
bool is_member_of(int vertex, vector<int> group){
    for(int i=0;i<group.size();i++){
        if(group[i] == vertex){
            return true;
        }
    }
    return false;
}

// removes vertex from group and returns new group
vector<int> remove_vertex_from_group(int vertex, vector<int> group){
    vector<int> new_group;
    for(int i=0;i<group.size();i++){
        if(group[i] != vertex){
            new_group.push_back(group[i]);
        }
    }
    return new_group;
}

#endif
