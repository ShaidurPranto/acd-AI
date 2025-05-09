#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <memory>

using namespace std;

int k;
int heuristicChoice;

int countLinearConflicts(vector<vector<int>> &state);

////////////////////////////////////////////// heuristic functions

double hammingDistance(vector<vector<int>> &state)
{
    double hamming = 0.0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (state[i][j] != 0 && state[i][j] != (i * k + j + 1))
            {
                hamming++;
            }
        }
    }
    return hamming;
}

double manhattanDistance(vector<vector<int>> &state)
{
    double manhattan = 0.0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {

            int val = state[i][j];
            if (val == 0) continue;

            int targetRow = ceil((val + 0.0) / (k + 0.0));
            int targetCol = val % k;
            if (targetCol == 0) targetCol = k;

            manhattan = manhattan + abs(targetCol - (j + 1)) + abs(targetRow - (i + 1));
        }
    }
    return manhattan;
}

double euclideanDistance(vector<vector<int>> &state)
{
    double euclidean = 0.0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {

            int val = state[i][j];
            if (val == 0) continue;

            int targetRow = ceil((val + 0.0) / (k + 0.0));
            int targetCol = val % k;
            if (targetCol == 0) targetCol = k;

            euclidean = euclidean + sqrt(((targetRow - (i + 1)) * (targetRow - (i + 1))) + ((targetCol - (j + 1)) * (targetCol - (j + 1))));
        }
    }
    return euclidean;
}

double linearConflictDistance(vector<vector<int>> &state)
{
    return manhattanDistance(state) + 2 * countLinearConflicts(state);
}

double getHeuristicValue(vector<vector<int>> &state)
{
    if(heuristicChoice == 1){
        return hammingDistance(state);
    }else if(heuristicChoice == 2){
        return manhattanDistance(state);
    }else if(heuristicChoice == 3){
        return euclideanDistance(state);
    }else if(heuristicChoice == 4){
        return linearConflictDistance(state);
    }else{
        return hammingDistance(state);
    }
}


////////////////////////////////////////////////// util - classes and structs

class Node
{
public:
    vector<vector<int>> board;
    int movesCount;
    // Node * parent;
    shared_ptr<Node> parent;

    Node()
    {
        movesCount = 0;
        parent = NULL;
    }

    bool operator<(const Node &other) const
    {
        return board < other.board;
    }
};

struct CompareNode
{
    bool operator()(const Node &a, const Node &b)
    {
        vector<vector<int>> vectorA = a.board;
        vector<vector<int>> vectorB = b.board;

        double costA = a.movesCount + getHeuristicValue(vectorA);
        double costB = b.movesCount + getHeuristicValue(vectorB);

        if(costA != costB ) return costA > costB;
        return getHeuristicValue(vectorA) > getHeuristicValue(vectorB);
    }
};


//////////////////////////////////////////////////////////// util- functions


int countInversionInLine(vector<int> arr) // returns number of inversions in a line
{
    int n = arr.size();
    int inversionCount = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (arr[i] > arr[j]){
                inversionCount++;
            }
        }
    }
    return inversionCount;
}

int countInversion(vector<vector<int>> &board) // returns number of inversions in a board
{
    vector<int> arr;
    int i = 0;
    for (vector<int> row : board)
    {
        for (int val : row)
        {
            if (val != 0)
            {
                arr.push_back(val);
            }
        }
    }

    return countInversionInLine(arr);
}

int countLinearConflicts(vector<vector<int>> &state) // returns number of linear conflicts in a board
{
    // row wise linear conflict
    int count = 0;
    vector<int> v;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            int val = state[i][j];
            if(val == 0) continue;

            int targetRow = ceil((val + 0.0) / (k + 0.0));
            if (i + 1 == targetRow){
                v.push_back(val);
            }
        }
        count = count + countInversionInLine(v);
        v.clear();
    }

    // column wise linear conflict
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            int val = state[j][i];
            if(val == 0) continue;

            int targetCol = val % k;
            if (targetCol == 0){
                targetCol = k;
            }

            if (j + 1 == targetCol){
                v.push_back(val);
            }
        }
        count = count + countInversionInLine(v);
        v.clear();
    }
    return count;
}

void printBoard(vector<vector<int>> &board)
{
    for (auto &row : board)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<Node> generateChildren(vector<vector<int>> &board)
{
    vector<Node> children;
    int x, y;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (board[i][j] == 0)
            {
                x = i;
                y = j;
            }
        }
    }

    // up
    if (x - 1 >= 0)
    {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x - 1][y]);
        children.push_back(child);
    }

    // down
    if (x + 1 < k)
    {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x + 1][y]);
        children.push_back(child);
    }

    // left
    if (y - 1 >= 0)
    {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x][y - 1]);
        children.push_back(child);
    }

    // right
    if (y + 1 < k)
    {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x][y + 1]);
        children.push_back(child);
    }

    return children;
}



///////////////////////////////////////////////////////////////////// tasks

bool isSolvable(vector<vector<int>> &board)
{
    int inversionCount = countInversion(board);
    if ((k % 2 == 1) && (inversionCount % 2 == 0))
    {
        return true;
    }

    int blankPos = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (board[i][j] == 0){
                blankPos = k - i;
                break;
            }
        }

        if (blankPos) break;
    }

    if ((k % 2 == 0) && (inversionCount % 2 != blankPos % 2)){
        return true;
    }

    return false;
}

void solve(vector<vector<int>> &board)
{

    vector<vector<int>> target(k, vector<int>(k, 0));
    int val = 1;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (val == k * k){
                target[i][j] = 0;
            }
            else{
                target[i][j] = val++;
            }
        }
    }

    int exploredCount = 0;
    int expandedCount = 0;

    Node initial;
    initial.board = board;

    priority_queue<Node, vector<Node>, CompareNode> q;
    map<Node, int> visited;

    q.push(initial);
    visited[initial] = 1;
    exploredCount++;

    long long int testCount = 1;

    while (!q.empty())
    {
        Node current = q.top();
        q.pop();

        cout<<" Testing node no. " << testCount++ << endl;

        if (current.board == target){
            // cout << "got the answer "<<endl;
            vector<Node> path;
            while (&current != NULL)
            {
                path.push_back(current);
                if (current.parent == NULL) break;
                current = *(current.parent);
            }

            cout << "Minimum number of moves: " << path.size() - 1 << endl;
            for (int i = path.size() - 1; i >= 0; i--)
            {
                cout << "Move no. " << path[i].movesCount << endl;
                printBoard(path[i].board);
            }

            cout << "Explored node count : " << exploredCount << endl;
            cout << "Expanded node count : " << expandedCount << endl;

            return;
        }

        vector<Node> children = generateChildren(current.board);
        for (Node &child : children)
        {
            if (visited[child] != 1){

                child.movesCount = current.movesCount + 1;

                //child.parent = new Node(current);
                child.parent = make_shared<Node>(current);

                q.push(child);
                visited[child] = 1;
                exploredCount++;
            }
        }
        expandedCount++;
    }

    cout << "No solution found.\n";
}

int main(int argc , char * argv[]) 
{
    if(argc == 2){
        heuristicChoice = atoi(argv[1]);
    }else{
        heuristicChoice = 1;
    }

    switch (heuristicChoice)
    {
    case 1:
        cout << "Using ' Hamming Distance ' as Heuristic Function "<<endl;
        break;
    case 2:
        cout << "Using ' Manhattan Distance ' as Heuristic Function "<<endl;
        break;
    case 3:
        cout << "Using ' Euclidean Distance ' as Heuristic Function "<<endl;
        break;
    case 4:
        cout << "Using ' Linear Conflict ' as Heuristic Function "<<endl;
        break;
    default:
        cout << "No heuristic function choosen "<<endl;
        break;
    }
    cout << endl;


    cout << "Enter Puzzle size and initial board configuartion " << endl;
    cin >> k;
    vector<vector<int>> board(k, vector<int>(k));
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            cin >> board[i][j];
        }
    }
    cout << endl << endl;

    // cout << "Trying to know if solvable "<<endl;

    if (!isSolvable(board))
    {
        cout << "Unsolvable puzzle." << endl;
        return 0;
    }

    // cout << "solvable , trying to solve "<<endl;

    solve(board);

    return 0;
}