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

int construct_random(Graph& g,int max_iterations){
    long long int total_weight = 0;
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
        int weight = weight_of_cut(g, cut);
        total_weight = total_weight + weight;
    }
    int average_weight = total_weight / max_iterations;
    return average_weight;
}


#endif