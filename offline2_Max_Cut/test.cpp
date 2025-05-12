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
const int end_index = 3;
const string input_path = "./graph_GRASP/set1/g";
const string file_type = ".rud";
const int grasp_number_of_iterations = 7;
const int random_number_of_iterations = 7;
const double alpha = 0.5;
const int number_of_iterations_local_search = 10;

void generate_single_report_for_construction();
void generate_single_report_for_local_search();
void generate_single_report_for_grasp();
void generate_Complete_report();
void generate_Complete_report_for_plot();


int main(){
    // generate_single_report_for_construction();
    // generate_single_report_for_local_search();
    // generate_single_report_for_grasp();

    // generate_Complete_report();
    // generate_Complete_report_for_plot();

    // freopen("./graph_GRASP/set1/g1.rud", "r", stdin);

    // int number_of_vertices , number_of_edges;
    // cin >> number_of_vertices >> number_of_edges;

    // cout << "trying to create graph"<<endl;
    // cout << "number of vertices: " << number_of_vertices << endl;
    // cout << "number of edges: " << number_of_edges << endl;

    // Graph g(number_of_vertices);
    // cout << "graph created" << endl;
    // g.number_of_edges = number_of_edges;
    // for(int i=0;i<number_of_edges;i++){
    //     int a, b, w;
    //     cin >> a >> b >> w;
    //     g.updateEdgeWeight(a-1, b-1, w);
    // }
    // cout << "input taken from file" << endl;
    // cout << endl;

    // int random_wieght = construct_random(g, random_number_of_iterations);
    // Cut greedy_cut = construct_greedy(g);
    // int greedy_weight = weight_of_cut(g, greedy_cut);
    // Cut semi_greedy_cut = construct_semi_greedy(g , alpha);
    // int semi_greedy_weight = weight_of_cut(g, semi_greedy_cut);
    
    // CutForLocalSearch cutForLocalSearch_greedy = local_search(g, greedy_cut);
    // int local_search_greedy_weight = weight_of_cut(g, cutForLocalSearch_greedy.cut);
    // int local_search_greedy_iterations = cutForLocalSearch_greedy.NumberOfIterations;
    // CutForLocalSearch cutForLocalSearch_semi_greedy = local_search(g, semi_greedy_cut);
    // int local_search_semi_greedy_weight = weight_of_cut(g, cutForLocalSearch_semi_greedy.cut);
    // int local_search_semi_greedy_iterations = cutForLocalSearch_semi_greedy.NumberOfIterations;

    // Cut grasp_cut = grasp(g, grasp_number_of_iterations , alpha);
    // int grasp_weight = weight_of_cut(g, grasp_cut);

    // cout << "random weight: " << random_wieght << endl;
    // cout << "greedy weight: " << greedy_weight << endl;
    // cout << "semi greedy weight: " << semi_greedy_weight << endl;
    // cout << endl;

    // cout << "local search greedy weight: " << local_search_greedy_weight << endl;
    // cout << "local search greedy iterations: " << local_search_greedy_iterations << endl;
    // cout << endl;

    // cout << "local search semi greedy weight: " << local_search_semi_greedy_weight << endl;
    // cout << "local search semi greedy iterations: " << local_search_semi_greedy_iterations << endl;
    // cout << endl;

    // cout << "grasp weight: " << grasp_weight << endl;
    // cout << "grasp iterations: " << grasp_number_of_iterations << endl;
    // cout << "grasp alpha: " << alpha << endl;

    return 0;
}


void generate_Complete_report(){
    string output_file = "test_report.csv"; // output file
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


void generate_single_report_for_grasp(){
    string output_file = "test_report_grasp.csv"; // output file
    ofstream fout(output_file);

    fout << " , , GRASP , , \n";
    string header = "Name , |V| or n , |E| or m , Number of iterations , Best Value\n";
    fout << header;

    // range of input files
    for(int i=strat_index;i<=end_index;i++){
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

        Cut cut = grasp(g, grasp_number_of_iterations , alpha);
        int weight = weight_of_cut(g, cut);

        fout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << "," << grasp_number_of_iterations << "," << weight << endl;

    }

    fout.close();

    // debug
    cout << "Report for grasp generated" << endl;
}


void generate_single_report_for_local_search(){
    string output_file = "test_report_local_search.csv"; // output file
    ofstream fout(output_file);

    fout << " , , Local Search , , \n";
    string header = "Name , |V| or n , |E| or m , Construct ,Simple local or local-1\n";
    string header2 = " , , , , Number of iterations , Best Value\n";
    fout << header << header2;

    // range of input files
    for(int i=strat_index;i<=end_index;i++){
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

        Cut cut = construct_semi_greedy(g, alpha); // construction algorithm choice
        int weight_before_local_search = weight_of_cut(g, cut);

        // debug
        // cout << "Weight of cut before local search: " << weight_of_cut(g, cut) << endl;

        CutForLocalSearch cutForLocalSearch = local_search(g, cut);
        int weight_after_local_search = weight_of_cut(g, cutForLocalSearch.cut);
        int number_of_iterations = cutForLocalSearch.NumberOfIterations;

        // debug
        // cout <<"Weight of cut after local search: " << weight_of_cut(g, cutForLocalSearch.cut) << endl;

        fout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << ","
         << weight_before_local_search << ","
         << number_of_iterations << "," << weight_after_local_search
         << endl;
    }

    fout.close();

    // debug
    cout << "Report for local search generated" << endl;
}


void generate_single_report_for_construction(){
    string output_file = "test_report_construct.csv"; // output file , construction algorithm choice
    ofstream fout(output_file);

    fout << " , , Constructive Algorithm , , \n";
    string header = "Name , |V| or n , |E| or m , Semi greedy-1\n";  // construction algorithm choice
    fout << header;

    // range of input files
    for(int i=strat_index;i<=end_index;i++){ 
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

        Cut cut = construct_semi_greedy(g,alpha); // construction algorithm choice
        int weight = weight_of_cut(g, cut);

        fout << "G" + to_string(i) + "," << number_of_vertices << "," << number_of_edges << "," << weight << endl;
    }

    fout.close();

    // debug
    cout << "Report for constructive algorithm(semi greedy) generated" << endl;
}