#ifndef LOCAL_GRASP_HPP
#define LOCAL_GRASP_HPP

#include <iostream>
#include <random>
#include <queue>

#include "graph.hpp"
#include "util.hpp"
#include "local_search.hpp"

using namespace std;

Cut grasp(Graph& g,int max_iterations,double alpha = 0){
    // debug
    cout << "starting GRASP" << endl;

    Cut bestCut;
    for(int i=0;i<max_iterations;i++){
        // debug
        // cout << "Iteration: " << i + 1 << endl;

        Cut cut = construct_semi_greedy(g , alpha);

        // debug
        // cout << "Weight of cut from construction (semi greedy): " << weight_of_cut(g, cut) << endl;

        CutForLocalSearch cutForLocalSearch = local_search(g, cut);

        // debug
        // cout << "Weight of cut from local search: " << weight_of_cut(g, cutForLocalSearch.cut) << endl;

        if(weight_of_cut(g, cutForLocalSearch.cut) > weight_of_cut(g, bestCut)){
            bestCut = cutForLocalSearch.cut;
        }
    }

    // debug
    cout << "Ending Grasp" << endl;

    return bestCut;
}


#endif