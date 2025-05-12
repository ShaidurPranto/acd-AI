#include <iostream>
#include <fstream>

#include "heuristic_semi_greedy.hpp"
#include "heuristic_greedy.hpp"
#include "heuristic_random.hpp"
#include "local_search.hpp"
#include "grasp.hpp"


using namespace std;

// const int strat_index = 1;
// const int end_index = 7;
// const string input_path = "./graph_GRASP/set1/in";
// const string file_type = ".txt";
// const int grasp_number_of_iterations = 10;

const int strat_index = 1;
const int end_index = 54;
const string input_path = "./graph_GRASP/set1/g";
const string file_type = ".rud";
const int grasp_number_of_iterations = 7;
const int random_number_of_iterations = 7;
const double alpha = 0.5;
const int number_of_iterations_local_search = 10;

void generate_Complete_report();


int main(){

    generate_Complete_report();

    return 0;
}


void generate_Complete_report(){
    string output_file = "report.csv"; // output file
    ofstream fout(output_file);

    string header = ",Problem, , ,Constructive Algorithm, , Local search , , GRASP \n";
    string header2 = "Name , |V| or n , |E| or m , randomized , greedy , semi-greedy ,Simple local or local-1 , ,GRASP-1 \n";
    string header3 = " , , , , , ,Number of iterations , Average Value , Number of iterations , Best value\n";    
    fout << header << header2 << header3;

    // range of input files
    for(int i=strat_index;i<=end_index;i++){

        // debug
        cout << "Taking input from file: " << i << endl;

        string input_file = input_path + to_string(i) + file_type; // input file
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

        // constructive algorithms
        int random_weight = construct_random(g, random_number_of_iterations);

        Cut heuristic_greedy_cut = construct_greedy(g);
        int greedy_weight = weight_of_cut(g, heuristic_greedy_cut);

        Cut heuristic_semi_greedy_cut = construct_semi_greedy(g , alpha);
        int semi_greedy_weight = weight_of_cut(g, heuristic_semi_greedy_cut);

        // debug
        // cout << "All three constructions are done with weights: " << random_weight << " , " << greedy_weight << " , " << semi_greedy_weight << endl;

        // local search
        long long int average_local_search_iterations = 0;
        long long int average_local_search_weight = 0;
        for(int i = 0; i < number_of_iterations_local_search; i++) {
            Cut temp_semi_greedy_cut = construct_semi_greedy(g, alpha);
            CutForLocalSearch cutForLocalSearch_semi_greedy = local_search(g, temp_semi_greedy_cut);
            average_local_search_iterations += cutForLocalSearch_semi_greedy.NumberOfIterations;
            average_local_search_weight += weight_of_cut(g, cutForLocalSearch_semi_greedy.cut);
        }
        average_local_search_iterations /= number_of_iterations_local_search;
        average_local_search_weight /= number_of_iterations_local_search;

        // debug
        // cout << "Local search is done with iterations: " << average_local_search_iterations << " and weight: " << average_local_search_weight << endl;

        // grasp
        Cut grasp_cut = grasp(g, grasp_number_of_iterations , alpha);
        int grasp_weight = weight_of_cut(g, grasp_cut);

        // debug
        // cout << "GRASP is done with weight: " << weight_of_cut(g, grasp_cut) << endl;

        fout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << "," 
             << random_weight << ","
             << greedy_weight << ","
             << semi_greedy_weight << ","
             << average_local_search_iterations << ","
             << average_local_search_weight << ","
             << grasp_number_of_iterations << ","
             << grasp_weight 
             << endl;
    }


    fout.close();

    // debug
    cout << "Complete report generated" << endl;
}

