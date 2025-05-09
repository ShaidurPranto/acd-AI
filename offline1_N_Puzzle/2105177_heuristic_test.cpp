#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

int k;

int countInversionInLine(vector<int> arr){
    int n = arr.size();
    int inversionCount = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                inversionCount++;
            }
        }
    }
    return inversionCount;
}


double hammingDistance(vector<vector<int>>& state) {
    double hamming = 0.0;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (state[i][j] != 0 && state[i][j] != (i * k + j + 1)) {
                hamming++;
            }
        }
    }
    return hamming;
}

double manhattanDistance(vector<vector<int>> &state) {
    double manhattan = 0.0;
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++){

            int val = state[i][j];
            if(val == 0) continue;

            int targetRow = ceil((val+0.0)/(k+0.0));
            int targetCol = val % k;
            if(targetCol == 0) targetCol = k;

            manhattan = manhattan + abs(targetCol - (j+1)) + abs(targetRow - (i+1));
        }
    }
    return manhattan;
}

double euclideanDistance(vector<vector<int>> &state) {
    double euclidean = 0.0;
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++){

            int val = state[i][j];
            if(val == 0) continue;

            int targetRow = ceil((val+0.0)/(k+0.0));
            int targetCol = val % k;
            if(targetCol == 0) targetCol = k;

            euclidean = euclidean + sqrt(((targetRow - (i+1))*(targetRow - (i+1)))+((targetCol - (j+1))*(targetCol - (j+1))));
        }
    }
    return euclidean;
}

int countLinearConflicts(vector<vector<int>> &state) {
    // row wise linear conflict 
    int count = 0;
    vector<int> v;
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            int val = state[i][j];
            if(val == 0) continue;

            int targetRow = ceil((val+0.0)/(k+0.0));
            if(i+1 == targetRow) v.push_back(val);
        }
        count = count + countInversionInLine(v);
        v.clear();
    }

    // column wise linear conflict
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            int val = state[j][i];
            if(val == 0) continue;

            int targetCol = val % k;
            if(targetCol == 0) targetCol = k;
            
            if(j+1 == targetCol) v.push_back(val);
        }
        count = count + countInversionInLine(v);
        v.clear();
    }
    return count;
}

double linearConflictDistance(vector<vector<int>> &state){
    return manhattanDistance(state) + 2 * countLinearConflicts(state);
}


int main(){
    k = 3;
    vector<vector<int>> board(k, vector<int>(k));
    board[0][0] = 7 ; board[0][1] = 2 ; board[0][2] = 4;
    board[1][0] = 6 ; board[1][1] = 0 ; board[1][2] = 5;
    board[2][0] = 8 ; board[2][1] = 3 ; board[2][2] = 1;




    cout<<"Hamming Distance : "<<hammingDistance(board)<<endl;
    cout<<"Manhattan Distance : "<<manhattanDistance(board)<<endl;
    cout<<"Euclidean Distance : "<<euclideanDistance(board)<<endl;
    cout<<"Linear Conflict Distance : "<<linearConflictDistance(board)<<endl;
    
    return 0;
}