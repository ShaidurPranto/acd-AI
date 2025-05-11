#ifndef HEURISTIC_SEMI_GREEDY_HPP
#define HEURISTIC_SEMI_GREEDY_HPP

#include <iostream>
#include <random>
#include <vector>

#include "graph.hpp"
#include "util.hpp"

using namespace std;


Cut construct_semi_greedy(Graph& g , double alpha = 0){
    random_device rd;
    mt19937 gen(rd());
    
    // debug
    cout << "starting semi greedy construction" << endl;

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

    int n = g.number_of_vertices;

    if (alpha == 0) alpha = ((get_random_int() % 100) / 100.0);

    // debug
    cout << "alpha: "<< alpha << endl;    

    while((cut.x.size() + cut.y.size()) < n){
        function_values.clear();
        function_values.assign(g.number_of_vertices, 0);
        weight_x.clear();
        weight_x.assign(g.number_of_vertices, 0);
        weight_y.clear();
        weight_y.assign(g.number_of_vertices, 0);

        // compute greedy function values
        vector<int> rcl;
        int minimum = INT32_MAX;
        int maximum = -1;
        for(int i = 0; i < g.number_of_vertices; i++){
            if(!taken[i]){
                int sigma_x = value_for_group(g, i, cut.x);
                int sigma_y = value_for_group(g, i, cut.y);

                function_values[i] = max(sigma_x, sigma_y);
                weight_x[i] = sigma_x;
                weight_y[i] = sigma_y;

                maximum = max(maximum, max(sigma_x, sigma_y));
                minimum = min(minimum, min(sigma_x, sigma_y));
            }
        }

        // compute limit and build rcl
        int limit = floor(minimum + alpha * (maximum - minimum));
        for(int i = 0; i < g.number_of_vertices; i++){
            if(!taken[i] && function_values[i] >= limit){
                rcl.push_back(i);
            }
        }

        // debug
        // cout << "RCL building done" << endl;
        
        // select vertex from rcl
        uniform_int_distribution<> dis(0, rcl.size() - 1);
        int random_index = dis(gen);
        int selected_vertex = rcl[random_index];

        // place vertex in cut
        if(weight_x[selected_vertex] >= weight_y[selected_vertex]){
            cut.y.push_back(selected_vertex);
        } else {
            cut.x.push_back(selected_vertex);
        }

        taken[selected_vertex] = true;

        //debug
        // cout << "total vertices in cut: " << cut.x.size() + cut.y.size() << endl;
    }

    // debug
    cout << "Ending semi greedy construction" << endl;

    return cut;
}


#endif