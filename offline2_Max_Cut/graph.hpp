#ifndef GRAPH_HPP
#define GRAPH_HPP

#include<iostream>  
#include<vector>

using namespace std;

struct Edge
{
    int vertex_a;
    int vertex_b;
    int weight;

    Edge(){
        this->vertex_a = this->vertex_b = this->weight = 0;
    }
};


struct Graph{
    int number_of_vertices;
    int number_of_edges;
    vector< vector<int> > weights;

    Graph(int n = 0){
        this->number_of_vertices = n;
        this->number_of_edges = 0;
        this->weights.resize(n, vector<int>(n, 0));
    }

    void updateEdgeWeight(int a, int b, int w){
        if(a >= this->number_of_vertices || b >= this->number_of_vertices){
            return;
        }
        this->weights[a][b] = w;
        this->weights[b][a] = w;
    }
};


struct Cut
{
    vector<int> x;
    vector<int> y;
    // int weight;

    Cut(){
        // this->weight = 0;
        x.resize(0);
        y.resize(0);
    }
};


struct CutForLocalSearch
{
    Cut cut;
    int NumberOfIterations;
    CutForLocalSearch(Cut cut , int NumberOfIterations){
        this->cut = cut;
        this->NumberOfIterations = NumberOfIterations;
    }
};




#endif