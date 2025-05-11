#ifndef HEURISTIC_GREEDY_HPP
#define HEURISTIC_GREEDY_HPP

#include <iostream>
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

    int n = g.number_of_vertices;

    for(int i = 0; i < g.number_of_vertices; i++){
        if(!taken[i]){
            int sigma_x = value_for_group(g, i, cut.x);
            int sigma_y = value_for_group(g, i, cut.y);

            if(sigma_x >= sigma_y){
                cut.y.push_back(i);
            } else {
                cut.x.push_back(i);
            }
            taken[i] = true;

        }
    }

    // debug
    cout << "Ending greedy construction" << endl;

    return cut;
}


#endif