#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <vector>

#include "2105177_game.hpp"

using namespace std;


// returns the maximizing player color
bool isMaximizingPlayer(Color playerColor);

// returns the opposite color
Color alterColor(Color playerColor);

void printRed(string msg);
void printGreen(string msg);
void printGame(State currentState, bool title = true, bool sep = true);

// check if the game is over
bool isGameOver(const State& state);

// get critical mass for a cell
int getCriticalMass(int row, int col, int rows, int cols);

// check if there is any chain reaction , if yes update the state
void checkChainReaction(State& state, int row, int col, Color playerColor);


void checkChainReaction(State& state, int row, int col, Color playerColor) {
    int rows = state.board.size();
    int cols = state.board[0].size();

    queue<pair<int, int>> toExplode;
    toExplode.push({row, col});

    while (!toExplode.empty()) {
        if (isGameOver(state)) {
            return;
        }

        pair<int, int> curr = toExplode.front();
        int r = curr.first;
        int c = curr.second;
        toExplode.pop();

        Cell& explodingCell = state.board[r][c];
        int criticalMass = getCriticalMass(r, c, rows, cols);

        if (explodingCell.numberOfOrbs < criticalMass) {
            continue; 
        }

        explodingCell.numberOfOrbs -= criticalMass;

        if (c - 1 >= 0) {
            Cell& leftCell = state.board[r][c - 1];
            leftCell.numberOfOrbs++;
            leftCell.orbColor = playerColor;
            if (leftCell.numberOfOrbs >= getCriticalMass(r, c - 1, rows, cols)) {
                toExplode.push({r, c - 1});
            }
        }
        if (c + 1 < cols) {
            Cell& rightCell = state.board[r][c + 1];
            rightCell.numberOfOrbs++;
            rightCell.orbColor = playerColor;
            if (rightCell.numberOfOrbs >= getCriticalMass(r, c + 1, rows, cols)) {
                toExplode.push({r, c + 1});
            }
        }
        if (r - 1 >= 0) {
            Cell& upCell = state.board[r - 1][c];
            upCell.numberOfOrbs++;
            upCell.orbColor = playerColor;
            if (upCell.numberOfOrbs >= getCriticalMass(r - 1, c, rows, cols)) {
                toExplode.push({r - 1, c});
            }
        }
        if (r + 1 < rows) {
            Cell& downCell = state.board[r + 1][c];
            downCell.numberOfOrbs++;
            downCell.orbColor = playerColor;
            if (downCell.numberOfOrbs >= getCriticalMass(r + 1, c, rows, cols)) {
                toExplode.push({r + 1, c});
            }
        }
    }
}
bool isMaximizingPlayer(Color playerColor) {
    return playerColor == getMaximizingPlayer();
}
Color alterColor(Color playerColor) {
    if (playerColor == RED) return GREEN;
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
void printGame(State currentState, bool title, bool sep) {
    int rows = currentState.board.size();
    int cols = currentState.board[0].size();

    if (sep) cout << "----------------------------------------------" << endl;

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
            Cell& cell = currentState.board[i][j];

            if (cell.numberOfOrbs == 0) {
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
bool isGameOver(const State& state) {
    bool hasRed = false;
    bool hasGreen = false;
    int count = 0;

    for (const auto& row : state.board) {
        for (const auto& cell : row) {
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
int getCriticalMass(int row, int col, int rows, int cols) {
    int mass = 0;
    if (col - 1 >= 0) mass++;
    if (col + 1 < cols) mass++;
    if (row - 1 >= 0) mass++;
    if (row + 1 < rows) mass++;
    return mass;
}


#endif