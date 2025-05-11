#ifndef HEURISTIC_GREEDY_HPP
#define HEURISTIC_GREEDY_HPP

#include <iostream>
#include <random>
#include <vector>

#include "graph.hpp"
#include "util.hpp"

using namespace std;


Cut construct_greedy(Graph& g){
    // debug
    cout << "starting greedy construction" << endl;

    Edge maxEdge = get_max_weight_edge(g);

    Cut cut;
    cut.x.push_back(maxEdge.vertex_a);
    cut.y.push_back(maxEdge.vertex_b);

    vector<bool> taken(g.number_of_vertices, false);
    taken[maxEdge.vertex_a] = true;
    taken[maxEdge.vertex_b] = true;

    vector<int> function_values(g.number_of_vertices, 0);
    vector<int> weight_x(g.number_of_vertices, 0);
    vector<int> weight_y(g.number_of_vertices, 0);

    int n = g.weights.size();

    while(cut.x.size() + cut.y.size() < n){
        function_values.clear();
        function_values.resize(g.number_of_vertices, 0);
        weight_x.clear();
        weight_x.resize(g.number_of_vertices, 0);
        weight_y.clear();
        weight_y.resize(g.number_of_vertices, 0);

        // compute greedy function values 
        int maxValueIndex = -1;
        for(int i = 0; i < g.number_of_vertices; i++){
            if(!taken[i]){
                int sigma_x = value_for_group(g, i, cut.x);
                int sigma_y = value_for_group(g, i, cut.y);

                function_values[i] = max(sigma_x, sigma_y);
                weight_x[i] = sigma_x;
                weight_y[i] = sigma_y;

                if(maxValueIndex == -1 || function_values[i] > function_values[maxValueIndex]){
                    maxValueIndex = i;
                }
            }
        }

        // place vertex in cut
        if(weight_x[maxValueIndex] >= weight_y[maxValueIndex]){
            cut.y.push_back(maxValueIndex);
        } else {
            cut.x.push_back(maxValueIndex);
        }

        taken[maxValueIndex] = true;

        //debug
        // cout << "total vertices in cut: " << cut.x.size() + cut.y.size() << endl;
    }

    // debug
    cout << "Ending greedy construction" << endl;

    return cut;
}


#endif