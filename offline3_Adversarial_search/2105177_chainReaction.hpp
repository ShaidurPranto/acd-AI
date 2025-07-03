#ifndef CHAIN_REACTION_HPP
#define CHAIN_REACTION_HPP

#include <iostream>
#include <queue>
#include <fstream>    
#include <thread>
#include <chrono>
#include <cstdio>

#include "2105177_game.hpp"
#include "2105177_util.hpp"
#include "2105177_minimax.hpp"

using namespace std;

class ChainReaction {
    int rows;
    int cols;
    Player Player1;
    Player Player2;
    State currentState;
    int numberOfMoves;
    MinimaxSearch minimax;
    string fileName;


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

    void flipTurn() {
        numberOfMoves++;
        currentState.turn = (currentState.turn == RED) ? GREEN : RED;
    }

    void makeMove(int row, int col, Color playerColor) {
        Cell& cell = currentState.board[row][col];
        cell.numberOfOrbs++;
        cell.orbColor = playerColor;

        // write updated board to file
        if(UI) updateFile();

        // check for chain reaction
        checkChainReaction(currentState, row, col, playerColor);

        // write updated board to file after chain reaction
        if(UI) updateFile();

        // flip turn
        flipTurn();

        // wait enough time before making another move
        if(UI && Player1.mode == AI && Player2.mode == AI) this_thread::sleep_for(chrono::milliseconds(TIME_DELAY_AMONG_TWO_AI_MOVES));
    }

    Color getWinner() {
        int redCount = 0;
        int greenCount = 0;

        for (const auto& row : currentState.board) {
            for (const auto& cell : row) {
                if (cell.orbColor == RED) {
                    redCount += cell.numberOfOrbs;
                }
                else if (cell.orbColor == GREEN) {
                    greenCount += cell.numberOfOrbs;
                }
            }
        }

        if (redCount == 0 && greenCount > 0) {
            return GREEN;
        }
        else if (greenCount == 0 && redCount > 0) {
            return RED;
        }
        else {
            return EMPTY;
        }
    }

    void takeHumanMoveFromUI(Player player) {
        cout << "Waiting for human move..." << endl;

        while (true) {
            ifstream gameStateFile("game_state.txt");
            if (gameStateFile.is_open()) {
                string headerLine;
                getline(gameStateFile, headerLine);
                gameStateFile.close();

                // check if the header indicates it's a human move
                if (headerLine.find("Human Move:") != string::npos) {
                    cout << "Human turn detected" << endl;
                    break;
                }
            }

            // wait a bit before checking again
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        // get the updated game state from the file
        vector< vector<Cell> > board = takeInputFromFile();

        // detect the cell with difference
        int row = -1, col = -1;
        for (int i = 0;i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                if (board[i][j].numberOfOrbs != this->currentState.board[i][j].numberOfOrbs) {
                    row = i;
                    col = j;
                    break;
                }
            }
        }

        cout << "HUMAN MOVE: " << row << " , " << col << endl;

        if (row == -1 || col == -1) {
            cout << "Invalid move detected, please try again." << endl;
            exit(1);
        }

        if(isValidMove(row, col, player.playerColor)) {
            makeMove(row, col, player.playerColor);
        }
        else {
            cout << "Invalid move detected from UI" << endl;
            exit(1);
        }
    }

    void takeHumanMoveFromConsole(Player player) {
        int row, col;
        cout << "Player " << (player.playerColor == RED ? "Red" : "Green") << ", enter your move (row and column): ";
        cin >> row >> col;

        // validate and make the move
        if (isValidMove(row, col, player.playerColor)) {
            makeMove(row, col, player.playerColor);
        }
        else {
            cout << "Invalid move. Try again." << endl;
            takeHumanMoveFromConsole(player);
        }
    }

    void takeHumanMove(Player player) {
        if (UI) {
            // take input from UI
            takeHumanMoveFromUI(player);
        }
        else {
            // take input from console
            takeHumanMoveFromConsole(player);
        }
    }

    void makeAIMove(Player player) {
        minimax.setState(this->currentState);
        minimax.setDepth(player.depth);
        minimax.setHeuristic(player.htype);

        Move move;

        if (isMaximizingPlayer(player.playerColor)) {
            // maximizing agent
            cout << "AI trying to make maximizing move" << endl;
        }
        else {
            // minimizing agent
            cout << "AI trying to make minimizing move" << endl;
        }

        move = minimax.minimax();

        int row = move.row;
        int col = move.col;
        int value = move.value;

        cout << "AI MOVE - value: " << value << " row: " << row << " col: " << col << endl;


        if (isValidMove(row, col, player.playerColor)) {
            makeMove(row, col, player.playerColor);
        }
        else {
            cout << "AI failed to make move" << endl;
            exit(1);
        }
    }

    // takes input from file and updates the board
    vector<vector<Cell>> takeInputFromFile() {
        ifstream inputFile(fileName);
        if (!inputFile) {
            cerr << "Error opening file: " << fileName << endl;
            return {};
        }

        // read header
        string header1, header2 = "";
        inputFile >> header1 >> header2;

        // read rows and columns
        vector< vector<Cell> > board(rows, vector<Cell>(cols, { 0, EMPTY }));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                string cell;
                inputFile >> cell;

                char num = cell[0];
                int numberOfOrbs = atoi(&num);
                char orbColor = 'E';
                if (cell.length() > 1) {
                    orbColor = cell[1];
                }

                board[i][j].numberOfOrbs = numberOfOrbs;
                if (orbColor == 'R') {
                    board[i][j].orbColor = RED;
                }
                else if (orbColor == 'G') {
                    board[i][j].orbColor = GREEN;
                }
                else {
                    board[i][j].orbColor = EMPTY;
                }
            }
        }

        inputFile.close();

        return board;
    }

    void initialWriteToFile() {
        ofstream outputFile(fileName);
        if (!outputFile) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

        // write header
        outputFile << (Player2.mode == HUMAN ? "Human" : "AI") << " Move:" << endl;

        // write rows and columns
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                outputFile << "0";
                outputFile << " ";
            }
            outputFile << endl;
        }

        outputFile.close();
    }

    void updateFile() {
        ofstream outputFile(fileName);
        if (!outputFile) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

        // write header
        if (numberOfMoves % 2 == 0) {
            outputFile << (Player1.mode == HUMAN ? "Human" : "AI") << " Move:" << endl;
        }
        else {
            outputFile << (Player2.mode == HUMAN ? "Human" : "AI") << " Move:" << endl;
        }

        // write rows and columns
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Cell& cell = currentState.board[i][j];
                outputFile << cell.numberOfOrbs;

                if (cell.numberOfOrbs != 0) {
                    if (cell.orbColor == RED) {
                        outputFile << "R";
                    }
                    else if (cell.orbColor == GREEN) {
                        outputFile << "G";
                    }
                }

                outputFile << " ";
            }
            outputFile << endl;
        }

        outputFile.close();
    }    

    void writeToFile() {
        updateFile();
        flipTurn();
    }


    public:

    ChainReaction(int rows, int cols, Player player1, Player player2, string filename) {
        this->rows = rows;
        this->cols = cols;

        currentState.board.resize(rows, vector<Cell>(cols, { 0, EMPTY }));
        currentState.turn = player1.playerColor;

        Player1 = player1;
        Player2 = player2;

        numberOfMoves = 0;

        this->fileName = filename;

        if (UI) initialWriteToFile();
    }

    void start() {
        while (!isGameOver(currentState)) {
            printGame(this->currentState);
            if (currentState.turn == Player1.playerColor) {
                // player 1's turn
                if (Player1.mode == HUMAN) {
                    takeHumanMove(Player1);
                }
                else {
                    // handle AI move
                    makeAIMove(Player1);
                }
            }
            else {
                // player 2's turn
                if (Player2.mode == HUMAN) {
                    takeHumanMove(Player2);
                }
                else {
                    // handle AI move
                    makeAIMove(Player2);
                }
            }
        }

        Color winner = getWinner();
        if (winner == RED) {
            printRed("Red wins!\n");

        }
        else if (winner == GREEN) {
            printGreen("Green wins!\n");
        }
        printGame(this->currentState, false);
    }
};

#endif