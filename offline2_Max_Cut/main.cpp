#include <iostream>

#include "heuristic_semi_greedy.hpp"
#include "heuristic_greedy.hpp"
#include "heuristic_random.hpp"
#include "local_search.hpp"


using namespace std;


void generate_single_report_for_construct_semi_greedy(){
    string output_file = "report_construct_semi_greedy.csv"; // output file
    freopen(output_file.c_str(), "w", stdout);
    string header = "Name , |V| or n , |E| or m , Simple Randomized or Randomized-1\n";
    cout << header;

    // range of input files
    for(int i=1;i<=3;i++){ 
        string input_file = "./graph_GRASP/set1/in" + to_string(i) + ".txt"; // input file
        freopen(input_file.c_str(), "r", stdin);
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

        cout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << "," << weight_of_cut(g, cut) << endl;

    }
}

void generate_single_report_for_local_search(){
    string output_file = "report_local_search.csv"; // output file
    freopen(output_file.c_str(), "w", stdout);
    string header = "Name , |V| or n , |E| or m ,Simple local or local-1\n";
    string header2 = " , , ,Number of iterations , Best Value\n";
    cout << header << header2;

    // range of input files
    for(int i=1;i<=3;i++){
        string input_file = "./graph_GRASP/set1/in" + to_string(i) + ".txt"; // input file
        freopen(input_file.c_str(), "r", stdin);
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

        // debug
        // cout << "Weight of cut before local search: " << weight_of_cut(g, cut) << endl;

        CutForLocalSearch cutForLocalSearch = local_search(g, cut);

        // debug
        // cout <<"Weight of cut after local search: " << weight_of_cut(g, cutForLocalSearch.cut) << endl;

        cout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << "," << cutForLocalSearch.NumberOfIterations << "," << weight_of_cut(g, cutForLocalSearch.cut) << endl;
    }
}


void generate_Complete_report(){
    string output_file = "report.csv"; // output file
    freopen(output_file.c_str(), "w", stdout);
    string header = ",Problem, , ,Constructive Algorithm, , Local search , \n";
    string header2 = "Name , |V| or n , |E| or m , randomized , greedy , semi-greedy ,Simple local or local-1\n";
    string header3 = " , , , , , ,Number of iterations , Best Value\n";    
    cout << header << header2 << header3;

    // range of input files
    for(int i=1;i<=3;i++){
        string input_file = "./graph_GRASP/set1/in" + to_string(i) + ".txt"; // input file
        freopen(input_file.c_str(), "r", stdin);
        int number_of_vertices , number_of_edges;
        cin >> number_of_vertices >> number_of_edges;

        Graph g(number_of_vertices);
        g.number_of_edges = number_of_edges;
        for(int i=0;i<number_of_edges;i++){
            int a, b, w;
            cin >> a >> b >> w;
            g.updateEdgeWeight(a-1, b-1, w);
        }

        Cut heuristic_random_cut = construct_random(g);
        Cut heuristic_greedy_cut = construct_greedy(g);
        Cut heuristic_semi_greedy_cut = construct_semi_greedy(g);

        CutForLocalSearch cutForLocalSearch_random = local_search(g, heuristic_random_cut);
        CutForLocalSearch cutForLocalSearch_greedy = local_search(g, heuristic_greedy_cut);
        CutForLocalSearch cutForLocalSearch_semi_greedy = local_search(g, heuristic_semi_greedy_cut);
        
        int total_iterations = cutForLocalSearch_random.NumberOfIterations + cutForLocalSearch_greedy.NumberOfIterations + cutForLocalSearch_semi_greedy.NumberOfIterations;
        double average_iterations = (double)total_iterations / 3.0;
        double average_weight = (double)(weight_of_cut(g, cutForLocalSearch_random.cut) + weight_of_cut(g, cutForLocalSearch_greedy.cut) + weight_of_cut(g, cutForLocalSearch_semi_greedy.cut)) / 3.0;
        double max_weight = max(weight_of_cut(g, cutForLocalSearch_random.cut), max(weight_of_cut(g, cutForLocalSearch_greedy.cut), weight_of_cut(g, cutForLocalSearch_semi_greedy.cut)));

        cout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << "," 
             << weight_of_cut(g, heuristic_random_cut) << ","
             << weight_of_cut(g, heuristic_greedy_cut) << ","
             << weight_of_cut(g, heuristic_semi_greedy_cut) << ","
             << total_iterations << ","
             << max_weight << 
             endl;
    }

}


int main(){
    generate_single_report_for_construct_semi_greedy();
    generate_single_report_for_local_search();
    generate_Complete_report();
    return 0;
}

