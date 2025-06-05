#ifndef CHAIN_REACTION_HPP
#define CHAIN_REACTION_HPP

#include <iostream>
#include <queue>    

#include "game.hpp"
#include "minimax.hpp"

using namespace std;

class ChainReaction {
    int rows;
    int cols;
    Player Player1;
    Player Player2;
    State currentState;
    int numberOfMoves;

    bool isGameOver(const State& state) {
        if(numberOfMoves <2) {
            return false;
        }

        bool hasRed = false;
        bool hasGreen = false;

        for (const auto& row : state.board) {
            for (const auto& cell : row) {
                if (cell.numberOfOrbs > 0) {
                    if (cell.orbColor == RED) hasRed = true;
                    else hasGreen = true;
                }
            }
        }

        // game overs when one player has no orbs left
        return !(hasRed && hasGreen);
    }

    bool isValidMove(int row, int col, Color playerColor) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            return false; 
        }
        Cell cell = currentState.board[row][col];
        if (cell.numberOfOrbs == 0 || cell.orbColor == playerColor) {
            return true;
        }
        return false;
    }

    void makeMove(int row, int col, Color playerColor) {
        Cell& cell = currentState.board[row][col];
        cell.numberOfOrbs++;
        cell.orbColor = playerColor;
        numberOfMoves++;
    }

    int getCriticalMass(int row, int col) {
        int mass = 0;
        if(col - 1 >= 0) mass++;
        if(col + 1 < cols) mass++;
        if(row - 1 >= 0) mass++;
        if(row + 1 < rows) mass++;
        return mass;
    }

    void checkChainReaction(int row, int col, Color playerColor) {
        int criticalMass = getCriticalMass(row, col);
        Cell& cell = currentState.board[row][col];

        if(cell.numberOfOrbs < criticalMass) {
            return; 
        }

        queue<pair<int, int>> toExplode;
        toExplode.push({row, col});
        
        while (!toExplode.empty()) {
            pair<int,int> curr = toExplode.front();
            int r = curr.first;
            int c = curr.second;
            toExplode.pop();

            Cell& explodingCell = currentState.board[r][c];
            int criticalMass = getCriticalMass(r, c);

            // explodingCell.numberOfOrbs = explodingCell.numberOfOrbs - criticalMass;
            explodingCell.numberOfOrbs = 0;  
            // explodingCell.orbColor = playerColor; 

            if(c - 1 >= 0) {
                Cell& leftCell = currentState.board[r][c - 1];
                leftCell.numberOfOrbs++;
                leftCell.orbColor = playerColor;
                if(leftCell.numberOfOrbs >= getCriticalMass(r, c - 1)) {
                    toExplode.push({r, c - 1});
                }
            }
            if(c + 1 < cols) {
                Cell& rightCell = currentState.board[r][c + 1];
                rightCell.numberOfOrbs++;
                rightCell.orbColor = playerColor;
                if(rightCell.numberOfOrbs >= getCriticalMass(r, c + 1)) {
                    toExplode.push({r, c + 1});
                }
            }
            if(r - 1 >= 0) {
                Cell& upCell = currentState.board[r - 1][c];
                upCell.numberOfOrbs++;
                upCell.orbColor = playerColor;
                if(upCell.numberOfOrbs >= getCriticalMass(r - 1, c)) {
                    toExplode.push({r - 1, c});
                }
            }
            if(r + 1 < rows) {
                Cell& downCell = currentState.board[r + 1][c];
                downCell.numberOfOrbs++;
                downCell.orbColor = playerColor;
                if(downCell.numberOfOrbs >= getCriticalMass(r + 1, c)) {
                    toExplode.push({r + 1, c});
                }
            }

        }
    }

    void takeHumanMove(Player player) {
        int row, col;
        cout << "Player " << (player.playerColor == RED ? "Red" : "Green") << ", enter your move (row and column): ";
        cin >> row >> col;

        // validate and make the move
        if (isValidMove(row, col, player.playerColor)) {
            makeMove(row, col, player.playerColor);
            checkChainReaction(row, col, player.playerColor);
            currentState.turn = (player.playerColor == RED) ? GREEN : RED;
        } else {
            cout << "Invalid move. Try again." << endl;
            takeHumanMove(player);
        }
    }

    void printRed(string msg) {
        // ANSI color codes
        const string RED_COLOR = "\033[31m";    // Red text
        const string RESET_COLOR = "\033[0m";   // Reset to default color
        
        cout << RED_COLOR << msg << RESET_COLOR;
    }

    void printGreen(string msg) {
        // ANSI color codes
        const string GREEN_COLOR = "\033[32m";  // Green text
        const string RESET_COLOR = "\033[0m";   // Reset to default color

        cout << GREEN_COLOR << msg << RESET_COLOR;
    }

    void printGame(bool title = true) {
        cout << "----------------------------------------------" << endl;
        if(title) {
            string current = (currentState.turn == RED ? "Red" : "Green");
            string header = current + "'s turn\n";
            if(currentState.turn == RED) {
                printRed(header);
            } else {
                printGreen(header);
            }
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Cell& cell = currentState.board[i][j];
                
                if (cell.orbColor == RED) {
                    printRed(to_string(cell.numberOfOrbs) + "R");
                } else if (cell.orbColor == GREEN) {
                    printGreen(to_string(cell.numberOfOrbs) + "G");
                } else {
                    // Empty cell - use default color
                    cout << cell.numberOfOrbs << "E";
                }
                cout << "  ";
            }
            cout << endl;
        }
    }

    Color getWinner() {
        int redCount = 0;
        int greenCount = 0;

        for (const auto& row : currentState.board) {
            for (const auto& cell : row) {
                if (cell.orbColor == RED) {
                    redCount += cell.numberOfOrbs;
                } else if (cell.orbColor == GREEN) {
                    greenCount += cell.numberOfOrbs;
                }
            }
        }

        if(redCount == 0 && greenCount > 0) {
            return GREEN;  
        } else if(greenCount == 0 && redCount > 0) {
            return RED;    
        } else {
            return EMPTY;
        }
    }

public:

    ChainReaction(int rows, int cols, Player player1, Player player2) {
        this->rows = rows;
        this->cols = cols;

        currentState.board.resize(rows, vector<Cell>(cols, {0, EMPTY}));
        currentState.turn = player1.playerColor; 

        Player1 = player1;
        Player2 = player2;

        numberOfMoves = 0;
    }

    void start() {
        while (!isGameOver(currentState)) {
            printGame();
            if(currentState.turn == Player1.playerColor) {
                // player 1's turn
                if (Player1.mode == HUMAN) {
                    takeHumanMove(Player1);
                } else {
                    // handle AI move
                }
            } else {
                // player 2's turn
                if (Player2.mode == HUMAN) {
                    takeHumanMove(Player2);
                } else {
                    // handle AI move
                }
            }
        }

        Color winner = getWinner();
        if(winner == RED) {
            printRed("Red wins!\n");
            
        } else if(winner == GREEN) {
            printGreen("Green wins!\n");
        } 
        printGame(false);
    }
};

#endif