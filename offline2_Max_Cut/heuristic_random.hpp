#ifndef HEURISTIC_RANDOM_HPP
#define HEURISTIC_RANDOM_HPP

#include <iostream>
#include <random>
#include <vector>

#include "graph.hpp"
#include "util.hpp"

using namespace std;

double getProbability(){
    double random_number = rand() % 100 / 100.0;
    return random_number;
}

Cut construct_random(Graph& g,int max_iterations){
    Cut bestCut;
    for(int i=0;i<max_iterations;i++){
        Cut cut;
        for(int j=0;j<g.number_of_vertices;j++){
            double random_number = getProbability();
            if(random_number >= 0.5){
                cut.x.push_back(j);
            }else{
                cut.y.push_back(j);
            }
        }
        if(weight_of_cut(g, cut) > weight_of_cut(g, bestCut)){
            bestCut = cut;
        }
    }
    return bestCut;
}


#endif