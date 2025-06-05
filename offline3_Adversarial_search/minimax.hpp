#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "game.hpp"
#include "heuristic.hpp"

using namespace std;

struct Move {
    int row;
    int col;
    
    Move() : row(-1), col(-1) {}
    Move(int r, int c) : row(r), col(c) {}
};

class MinimaxSearch {
private:
    Heuristic heuristic;
    int heuristicType;
    State currentState;
    
    // Helper function to get critical mass for a cell
    int getCriticalMass(int row, int col, int rows, int cols) {
        if ((row == 0 || row == rows - 1) && (col == 0 || col == cols - 1)) {
            return 2; // Corner
        }
        else if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1) {
            return 3; // Edge
        }
        else {
            return 4; // Interior
        }
    }
    
    // Helper function to apply explosion and chain reactions
    State applyExplosion(State state, int row, int col) {
        int rows = state.board.size();
        int cols = state.board[0].size();
        
        // Queue for processing explosions
        vector<pair<int, int>> explosionQueue;
        explosionQueue.push_back({row, col});
        
        while (!explosionQueue.empty()) {
            auto [curRow, curCol] = explosionQueue.front();
            explosionQueue.erase(explosionQueue.begin());
            
            Cell& currentCell = state.board[curRow][curCol];
            int criticalMass = getCriticalMass(curRow, curCol, rows, cols);
            
            if (currentCell.numberOfOrbs >= criticalMass) {
                Color explodingColor = currentCell.orbColor;
                
                // Reduce orbs in current cell
                currentCell.numberOfOrbs -= criticalMass;
                if (currentCell.numberOfOrbs == 0) {
                    // Cell becomes empty, no color
                }
                
                // Distribute orbs to adjacent cells
                vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
                
                for (auto [dr, dc] : directions) {
                    int newRow = curRow + dr;
                    int newCol = curCol + dc;
                    
                    // Check bounds
                    if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                        Cell& adjacentCell = state.board[newRow][newCol];
                        
                        // Add orb to adjacent cell
                        adjacentCell.numberOfOrbs++;
                        adjacentCell.orbColor = explodingColor; // Convert color
                        
                        // Check if this cell now needs to explode
                        int adjCriticalMass = getCriticalMass(newRow, newCol, rows, cols);
                        if (adjacentCell.numberOfOrbs >= adjCriticalMass) {
                            // Add to explosion queue if not already there
                            bool alreadyQueued = false;
                            for (auto& queued : explosionQueue) {
                                if (queued.first == newRow && queued.second == newCol) {
                                    alreadyQueued = true;
                                    break;
                                }
                            }
                            if (!alreadyQueued) {
                                explosionQueue.push_back({newRow, newCol});
                            }
                        }
                    }
                }
            }
        }
        
        return state;
    }
    
    // Generate all possible moves for current player
    vector<Move> generateMoves(const State& state) {
        vector<Move> moves;
        int rows = state.board.size();
        int cols = state.board[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                const Cell& cell = state.board[i][j];
                
                // Can place orb if cell is empty or contains orbs of same color
                if (cell.numberOfOrbs == 0 || cell.orbColor == state.turn) {
                    moves.push_back(Move(i, j));
                }
            }
        }
        
        return moves;
    }
    
    // Apply a move to the state
    State applyMove(State state, int row, int col) {
        // Place orb
        state.board[row][col].numberOfOrbs++;
        state.board[row][col].orbColor = state.turn;
        
        // Apply explosions
        state = applyExplosion(state, row, col);
        
        // Switch turn
        state.turn = (state.turn == RED) ? GREEN : RED;
        
        return state;
    }
    
    // Check if game is over
    bool isTerminal(const State& state) {
        bool hasRed = false, hasGreen = false;
        
        for (const auto& row : state.board) {
            for (const auto& cell : row) {
                if (cell.numberOfOrbs > 0) {
                    if (cell.orbColor == RED) hasRed = true;
                    else hasGreen = true;
                }
            }
        }
        
        // Game is terminal if only one player has orbs (or no orbs at all)
        return !(hasRed && hasGreen);
    }
    
    // Internal minimax function for maximizing player
    pair<int, Move> minimaxMax(State state, int depth, Color playerColor, int alpha = numeric_limits<int>::min(), int beta = numeric_limits<int>::max()) {
        // Base case: terminal state or depth limit reached
        if (depth == 0 || isTerminal(state)) {
            int value = heuristic.evaluate(state, playerColor, heuristicType);
            return {value, Move()};
        }
        
        int bestValue = numeric_limits<int>::min();
        Move bestMove;
        
        vector<Move> moves = generateMoves(state);
        
        for (const Move& move : moves) {
            State newState = applyMove(state, move.row, move.col);
            auto [value, _] = minimaxMin(newState, depth - 1, playerColor, alpha, beta);
            
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
            
            // Alpha-beta pruning
            alpha = max(alpha, value);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        
        return {bestValue, bestMove};
    }
    
    // Internal minimax function for minimizing player
    pair<int, Move> minimaxMin(State state, int depth, Color playerColor, int alpha = numeric_limits<int>::min(), int beta = numeric_limits<int>::max()) {
        // Base case: terminal state or depth limit reached
        if (depth == 0 || isTerminal(state)) {
            int value = heuristic.evaluate(state, playerColor, heuristicType);
            return {value, Move()};
        }
        
        int bestValue = numeric_limits<int>::max();
        Move bestMove;
        
        vector<Move> moves = generateMoves(state);
        
        for (const Move& move : moves) {
            State newState = applyMove(state, move.row, move.col);
            auto [value, _] = minimaxMax(newState, depth - 1, playerColor, alpha, beta);
            
            if (value < bestValue) {
                bestValue = value;
                bestMove = move;
            }
            
            // Alpha-beta pruning
            beta = min(beta, value);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        
        return {bestValue, bestMove};
    }

public:
    MinimaxSearch(int hType = 5) : heuristicType(hType) {}
    
    // Set the current game state
    void setState(const State& state) {
        currentState = state;
    }
    
    // Public minimax_max function - only takes playerColor and depth
    Move minimax_max(Color playerColor, int depth) {
        auto [value, move] = minimaxMax(currentState, depth, playerColor);
        return move;
    }
    
    // Public minimax_min function - only takes playerColor and depth  
    Move minimax_min(Color playerColor, int depth) {
        auto [value, move] = minimaxMin(currentState, depth, playerColor);
        return move;
    }
    
    // Set heuristic type
    void setHeuristic(int hType) {
        heuristicType = hType;
    }
};

#endif