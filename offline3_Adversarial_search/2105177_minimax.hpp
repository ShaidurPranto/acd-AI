#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#include "2105177_game.hpp"
#include "2105177_util.hpp"
#include "2105177_heuristic.hpp"

using namespace std;


class MinimaxSearch {
private:
    Heuristic heuristic;
    int heuristicType;
    int depth;
    State currentState;
    
    // generate all possible moves for current player
    vector<Move> generateMoves(const State& state) {
        vector<Move> moves;
        int rows = state.board.size();
        int cols = state.board[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                const Cell& cell = state.board[i][j];
                
                // can place orb if cell is empty or contains orbs of same color
                if (cell.numberOfOrbs == 0 || cell.orbColor == state.turn) {
                    moves.push_back(Move(i, j));
                }
            }
        }
        
        return moves;
    }
    
    // apply a move to the state
    State applyMove(State state, int row, int col) {
        // place orb
        state.board[row][col].numberOfOrbs++;
        state.board[row][col].orbColor = state.turn;

        // apply explosions
        checkChainReaction(state,row,col,state.turn);
        
        // switch turn
        state.turn = (state.turn == RED) ? GREEN : RED;
        
        return state;
    }

    Move minimax_search(State state, int depth, int alpha, int beta) {
        if(debug()) {
            // debug
            cout << "\nMinimax search at depth: " << depth << endl << "for player: " << (state.turn == RED ? "Red" : "Green") << endl;
            printGame(state, false, false);
        }

        if (depth == 0 || isGameOver(state)) {
            int value = heuristic.evaluate(state,heuristicType);

            if(debug()) {
                //debug
                cout << "Reached leaf node or depth limit. Evaluating state with value: " << value << endl;
            }

            return Move(value);
        }

        vector<Move> moves = generateMoves(state);
        Move bestMove;

        if (isMaximizingPlayer(state.turn)) {
            int maxEval = numeric_limits<int>::min();

            for (const Move& move : moves) {
                State child = applyMove(state, move.row, move.col);

                if(debug()) {
                    // debug
                    cout << "\ncurrent child is " << endl;
                    printGame(child, false, false);
                }

                Move tempMove = minimax_search(child, depth-1, alpha, beta);
                tempMove.row = move.row;
                tempMove.col = move.col;

                // debug 
                if(debug()) cout << "Evaluating move at row: " << move.row << ", col: " << move.col << " with value: " << tempMove.value << endl;
                
                if (tempMove.value > maxEval) {
                    // debug
                    if(debug()) cout << "New best move found at row: " << move.row << ", col: " << move.col << " with value: " << tempMove.value << endl;

                    maxEval = tempMove.value;
                    bestMove = tempMove;
                }

                alpha = max(alpha, tempMove.value);
                if (beta <= alpha) {
                    // debug
                    if(debug()) cout << "Pruning branches at row: " << move.row << ", col: " << move.col << " with alpha: " << alpha << " and beta: " << beta << endl;

                    break;
                }
            }
        } else {
            int minEval = numeric_limits<int>::max();

            for(const Move& move: moves) {
                State child = applyMove(state, move.row, move.col);

                if(debug()) {
                    // debug
                    cout << "current child is " << endl;
                    printGame(child, false, false);
                }

                Move tempMove = minimax_search(child, depth-1, alpha, beta);
                tempMove.row = move.row;
                tempMove.col = move.col;
                
                // debug
                if(debug()) cout << "Evaluating move at row: " << move.row << ", col: " << move.col << " with value: " << tempMove.value << endl;

                if (tempMove.value < minEval) {
                    // debug
                    if(debug()) cout << "New best move found at row: " << move.row << ", col: " << move.col << " with value: " << tempMove.value << endl;

                    minEval = tempMove.value;
                    bestMove = tempMove;
                }

                beta = min(beta, tempMove.value);
                if (beta <= alpha) {
                    // debug
                    if(debug()) cout << "Pruning branches at row: " << move.row << ", col: " << move.col << " with alpha: " << alpha << " and beta: " << beta << endl;

                    break;
                }
            }
        }

        return bestMove;
    }

    public:
    MinimaxSearch(int depth = 3,int hType = 5) {
        this->depth = depth;
        this->heuristicType = hType;
    }

    // set depth of minimax search
    void setDepth(int depth) {
        this->depth = depth;
    }

    // set heuristic type
    void setHeuristic(int hType) {
        this->heuristicType = hType;
    }

    // set the current game state
    void setState(State state) {
        this->currentState = state;
    }

    // minimax search 
    Move minimax(bool randomMove = false) {
        if (randomMove) {
            // generate a random move
            vector<Move> moves = generateMoves(currentState);
            if (moves.empty()) {
                return Move(); // no valid moves
            }
            int randomIndex = rand() % moves.size();
            return moves[randomIndex];
        }
        int alpha = numeric_limits<int>::min();
        int beta = numeric_limits<int>::max();
        return minimax_search(currentState,depth,alpha,beta);
    }
};


#endif