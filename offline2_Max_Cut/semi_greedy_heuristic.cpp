#include <iostream>
#include <random>
#include <vector>

#include "graph.hpp"
#include "util.hpp"

using namespace std;


Cut construct_semi_greedy(Graph g){
    Edge maxEdge = get_max_weight_edge(g);
    cout << "Max edge: " << maxEdge.vertex_a + 1 << " , " << maxEdge.vertex_b + 1 << " with weight: " << maxEdge.weight << endl;
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

    double alpha = 0.5;

    while(cut.x.size() + cut.y.size() < n){
        function_values.clear();
        function_values.resize(g.number_of_vertices, 0);
        weight_x.clear();
        weight_x.resize(g.number_of_vertices, 0);
        weight_y.clear();
        weight_y.resize(g.number_of_vertices, 0);

        // compute greedy function values
        vector<int> rcl;
        int minimum = -1;
        int maximum = -1;
        for(int i = 0; i < g.number_of_vertices; i++){
            if(!taken[i]){
                int sigma_x = value_for_group(g, i, cut.x);
                int sigma_y = value_for_group(g, i, cut.y);
                
                if(maximum == -1){
                    maximum = max(sigma_x, sigma_y);
                }
                if(minimum == -1){
                    minimum = min(sigma_x, sigma_y);
                }

                function_values[i] = max(sigma_x, sigma_y) - minimum;
                weight_x[i] = sigma_x;
                weight_y[i] = sigma_y;

                if(maximum < max(sigma_x, sigma_y)){
                    maximum = max(sigma_x, sigma_y);
                }
                if(minimum > min(sigma_x, sigma_y)){
                    minimum = min(sigma_x, sigma_y);
                }
            }
        }

        // compute limit and build rcl
        int limit = floor(minimum + alpha * (maximum - minimum));
        for(int i = 0; i < g.number_of_vertices; i++){
            if(!taken[i] && function_values[i] >= limit){
                rcl.push_back(i);
            }
        }

        // adjust alpha if needed
        if(rcl.size() == 0){
            alpha -= 0.1;
            continue;
        }else{
            alpha = 0.5;
        }

        // cout << "RCL building done" << endl;

        // select vertex from rcl
        int selected_vertex = rcl[random() % rcl.size()];

        // place vertex in cut
        if(weight_x[selected_vertex] >= weight_y[selected_vertex]){
            cut.y.push_back(selected_vertex);
        } else {
            cut.x.push_back(selected_vertex);
        }

        taken[selected_vertex] = true;

        cout << "total vertices in cut: " << cut.x.size() + cut.y.size() << endl;
    }

    return cut;
}



int main(){
    freopen("./graph_GRASP/set1/g1.rud", "r", stdin);

    int number_of_vertices , number_of_edges;
    cin >> number_of_vertices >> number_of_edges;

    Graph g;
    g.number_of_vertices = number_of_vertices;
    g.number_of_edges = number_of_edges;
    g.weights.resize(number_of_vertices);
    for(int i=0;i<number_of_vertices;i++){
        g.weights[i].resize(number_of_vertices);
    }
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

    return 0;
}