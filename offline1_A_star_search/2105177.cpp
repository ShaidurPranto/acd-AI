#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

int k;

class Node {
public:
    vector<vector<int>> board;
    int movesCount;
    Node* parent;

    Node() {
        movesCount = 0;
        parent = NULL;
    }

    bool operator<(const Node& other) const {
        return board < other.board; 
    }
};

class Heuristic {
    static double hammingDistance(vector<vector<int>>& state) {
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

public:
    static double getHeuristicValue(vector<vector<int>>& state) {
        return hammingDistance(state);
    }
};

struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        vector<vector<int>> vectorA = a.board;
        vector<vector<int>> vectorB = b.board;
        double costA = a.movesCount + Heuristic::getHeuristicValue(vectorA);
        double costB = b.movesCount + Heuristic::getHeuristicValue(vectorB);
        return costA > costB;
    }
};

vector<Node> generateChildren(vector<vector<int>>& board) {
    vector<Node> children;
    int x, y;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (board[i][j] == 0) {
                x = i;
                y = j;
            }
        }
    }

    // up
    if (x - 1 >= 0) {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x - 1][y]);
        children.push_back(child);
    }

    // down
    if (x + 1 < k) {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x + 1][y]);
        children.push_back(child);
    }

    // left
    if (y - 1 >= 0) {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x][y - 1]);
        children.push_back(child);
    }

    // right
    if (y + 1 < k) {
        Node child;
        child.board = board;
        swap(child.board[x][y], child.board[x][y + 1]);
        children.push_back(child);
    }

    return children;
}

void solve(vector<vector<int>>& board) {
    priority_queue<Node, vector<Node>, CompareNode> q;

    Node initial;
    initial.board = board;
    q.push(initial);

    vector<vector<int>> target(k, vector<int>(k, 0));
    int val = 1;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (val == k * k){
                target[i][j] = 0;
            } else {
                target[i][j] = val++;
            }
        }
    }

    map<Node, bool> visited;

    while (!q.empty()) {
        Node current = q.top();
        q.pop();

        if (current.board == target) {

            vector<Node> path;
            while (&current != NULL) {
                path.push_back(current);
                // if(&current == current.parent){
                //     cout<<" Ohh fuck! I am stuck in a loop! "<<endl;
                // }
                if (current.parent == NULL) break;
                current = *(current.parent);
            }

            for (int i = path.size() - 1; i >= 0; i--) {
                cout << "Step " << path.size() - i - 1 << ":\n";
                for (int r = 0; r < k; r++) {
                    for (int c = 0; c < k; c++) {
                        cout << path[i].board[r][c] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }

            cout << "Total moves: " << path.size() - 1 << endl;
            return;
        }

        visited[current] = true;

        vector<Node> children = generateChildren(current.board);
        for (Node& child : children) {
            if (visited.find(child) == visited.end()) {
                child.movesCount = current.movesCount + 1;

                child.parent = new Node(current);
                // child.parent = &current;
                
                q.push(child);
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    cout << "Enter puzzle size : "; 
    cin >> k;

    vector<vector<int>> board(k, vector<int>(k));
    cout << "Enter initial puzzle board (use 0 for empty):\n";
    for (int i = 0; i < k; i++){
        for (int j = 0; j < k; j++){
            cin >> board[i][j];
        }
    }
    cout << endl << endl;

    solve(board);

    return 0;
}
