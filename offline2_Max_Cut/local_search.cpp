#include <iostream>
#include <random>
#include <queue>

#include "graph.hpp"
#include "util.hpp"
#include "heuristic_semi_greedy.hpp"

using namespace std;

struct TempNode{
    int vertex;
    int neighbor_changing_value;
};

struct custom_compare{
    bool operator()(const TempNode& a, const TempNode& b){
        return a.neighbor_changing_value < b.neighbor_changing_value;
    }
};

CutForLocalSearch local_search(Graph g, Cut cut){
    priority_queue<TempNode, vector<TempNode>, custom_compare> pq;
    int number_of_iterations = 0;

    while(true){
        pq = priority_queue<TempNode, vector<TempNode>, custom_compare>();
        for(int i=0;i<g.weights.size();i++){
            int value_x = value_for_group(g, i, cut.x);
            int value_y = value_for_group(g, i, cut.y);
            int neighbor_changing_value_x;
            if(is_member_of(i, cut.x)){
                neighbor_changing_value_x = value_x - value_y;
            } else {
                neighbor_changing_value_x = value_y - value_x;
            }
            TempNode node;
            node.vertex = i;
            node.neighbor_changing_value = neighbor_changing_value_x;
            pq.push(node);

            // debug
            // cout << "neighbor_changing_value of " << i + 1 << " is " << neighbor_changing_value_x << endl;
        }
        TempNode node = pq.top();
        pq.pop();
        if(node.neighbor_changing_value <= 0){
            break;
        }else{
            number_of_iterations++;

            if(is_member_of(node.vertex, cut.x)){
                cut.x = remove_vertex_from_group(node.vertex, cut.x);
                cut.y.push_back(node.vertex);
            } else {
                cut.y = remove_vertex_from_group(node.vertex, cut.y);
                cut.x.push_back(node.vertex);
            }
        }

    }

    // debug
    // cout << "Number of iterations: " << number_of_iterations << endl;
    
    return CutForLocalSearch(cut, number_of_iterations);
}



int main(){
    freopen("./graph_GRASP/set1/in1.txt", "r", stdin);

    int number_of_vertices , number_of_edges;
    cin >> number_of_vertices >> number_of_edges;

    Graph g(number_of_vertices);
    g.number_of_edges = number_of_edges;
    for(int i=0;i<number_of_edges;i++){
        int a, b, w;
        cin >> a >> b >> w;
        g.updateEdgeWeight(a-1, b-1, w);
    }
    Cut cut = construct_semi_greedy(g);
    cout << "X: ";
    for(int i=0;i<cut.x.size();i++){
        cout << cut.x[i] + 1 << " ";
    }
    cout << endl;
    cout << "Y: ";
    for(int i=0;i<cut.y.size();i++){
        cout << cut.y[i] + 1 << " ";
    }
    cout << endl;
    cout << "Weight of cut: " << weight_of_cut(g, cut) << endl;

    CutForLocalSearch cutforlocal = local_search(g, cut);

    cout << endl << endl;
    cout << "after local search , the updated cut is :" << endl;
    cout << "X: ";
    for(int i=0;i<cutforlocal.cut.x.size();i++){
        cout << cutforlocal.cut.x[i] + 1 << " ";
    }
    cout << endl;
    cout << "Y: ";
    for(int i=0;i<cutforlocal.cut.y.size();i++){
        cout << cutforlocal.cut.y[i] + 1 << " ";
    }
    cout << endl;
    cout << "Weight of cut: " << weight_of_cut(g, cutforlocal.cut) << endl;
    cout << "Number of iterations: " << cutforlocal.NumberOfIterations << endl;
    return 0;
}