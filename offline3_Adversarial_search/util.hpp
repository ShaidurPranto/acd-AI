#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <vector>

#include "game.hpp"

using namespace std;

// returns true if player is the maximizing player
bool isMaximizingPlayer(Color playerColor) {
    return playerColor == getMaximizingPlayer();
}

// returns opposite color
Color alterColor(Color playerColor) {
    if(playerColor == RED) return GREEN;
    return RED;
}

void printRed(string msg) {
    // ANSI color codes
    const string RED_COLOR = "\033[31m";  // Red text
    const string RESET_COLOR = "\033[0m"; // Reset to default color

    cout << RED_COLOR << msg << RESET_COLOR;
}

void printGreen(string msg) {
    // ANSI color codes
    const string GREEN_COLOR = "\033[32m"; // Green text
    const string RESET_COLOR = "\033[0m";  // Reset to default color

    cout << GREEN_COLOR << msg << RESET_COLOR;
}

// prints game status
void printGame(State currentState, bool title = true , bool sep = true) {
    int rows = currentState.board.size();
    int cols = currentState.board[0].size();

    if(sep) cout << "----------------------------------------------" << endl;

    if (title) {
        string current = (currentState.turn == RED ? "Red" : "Green");
        string header = current;
        if (currentState.turn == RED) {
            printRed(header);
        }
        else {
            printGreen(header);
        }
        cout << "'s turn\n";
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Cell &cell = currentState.board[i][j];
            
            if(cell.numberOfOrbs == 0) {
                cout << cell.numberOfOrbs << "E";
            }
            else if (cell.orbColor == RED) {
                printRed(to_string(cell.numberOfOrbs) + "R");
            }
            else if (cell.orbColor == GREEN) {
                printGreen(to_string(cell.numberOfOrbs) + "G");
            }
            cout << "  ";
        }
        cout << endl;
    }
}

// check if the game is over
bool isGameOver(const State &state) {
    bool hasRed = false;
    bool hasGreen = false;
    int count = 0;

    for (const auto &row : state.board) {
        for (const auto &cell : row) {
            if (cell.numberOfOrbs > 0) {
                count++;
                if (cell.orbColor == RED) {
                    hasRed = true;
                }
                else {
                    hasGreen = true;
                }
            }
        }
    }

    if (count < 2) return false;

    // game overs when one player has no orbs left
    return !(hasRed && hasGreen);
}

// get critical mass for a cell
int getCriticalMass(int row, int col, int rows, int cols) {
    int mass = 0;
    if(col - 1 >= 0) mass++;
    if(col + 1 < cols) mass++;
    if(row - 1 >= 0) mass++;
    if(row + 1 < rows) mass++;
    return mass;
}

// check and create if there is any chain reaction
void checkChainReaction(State &state, int row, int col, Color playerColor) {
    int rows = state.board.size();
    int cols = state.board[0].size();

    int criticalMass = getCriticalMass(row, col, rows, cols);
    Cell& cell = state.board[row][col];

    if (cell.numberOfOrbs < criticalMass) {
        return;
    }

    // debug
    if(debug()) cout << "critical mass surpassed" << endl;
    int count = 0;

    queue<pair<int, int>> toExplode;
    toExplode.push({ row, col });

    while (!toExplode.empty()) {
        if(debug() || 1) {
            // debug
            cout << "chain explosion no: " << count++ << endl;
        }
        if(isGameOver(state)) {
            if(debug()) {
                // debug
                cout << "game is already over , no further chain reaction is required" << endl;
            }
            return;
        }

        pair<int, int> curr = toExplode.front();
        int r = curr.first;
        int c = curr.second;
        toExplode.pop();

        Cell& explodingCell = state.board[r][c];
        int criticalMass = getCriticalMass(r, c, rows, cols);

        explodingCell.numberOfOrbs = 0;

        if (c - 1 >= 0) {
            Cell& leftCell = state.board[r][c - 1];
            leftCell.numberOfOrbs++;
            leftCell.orbColor = playerColor;
            if (leftCell.numberOfOrbs >= getCriticalMass(r, c - 1, rows, cols)) {
                toExplode.push({ r, c - 1 });
            }
        }
        if (c + 1 < cols) {
            Cell& rightCell = state.board[r][c + 1];
            rightCell.numberOfOrbs++;
            rightCell.orbColor = playerColor;
            if (rightCell.numberOfOrbs >= getCriticalMass(r, c + 1, rows, cols)) {
                toExplode.push({ r, c + 1 });
            }
        }
        if (r - 1 >= 0) {
            Cell& upCell = state.board[r - 1][c];
            upCell.numberOfOrbs++;
            upCell.orbColor = playerColor;
            if (upCell.numberOfOrbs >= getCriticalMass(r - 1, c, rows, cols)) {
                toExplode.push({ r - 1, c });
            }
        }
        if (r + 1 < rows) {
            Cell& downCell = state.board[r + 1][c];
            downCell.numberOfOrbs++;
            downCell.orbColor = playerColor;
            if (downCell.numberOfOrbs >= getCriticalMass(r + 1, c, rows, cols)) {
                toExplode.push({ r + 1, c });
            }
        }

        // debug
        if(debug()) printGame(state,false,false);

    }
}

#endif