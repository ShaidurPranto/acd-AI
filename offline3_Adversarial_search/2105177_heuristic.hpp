#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "2105177_game.hpp"
#include "2105177_util.hpp"

using namespace std;

class Heuristic {

    // Helper function to count orbs for each player
    pair<int, int> countOrbs(const State& state) {
        int redOrbs = 0, greenOrbs = 0;
        
        for (const auto& row : state.board) {
            for (const auto& cell : row) {
                if (cell.numberOfOrbs > 0) {
                    if (cell.orbColor == RED) {
                        redOrbs += cell.numberOfOrbs;
                    } else {
                        greenOrbs += cell.numberOfOrbs;
                    }
                }
            }
        }
        return {redOrbs, greenOrbs};
    }
    
    // Helper function to count cells owned by each player
    pair<int, int> countCells(const State& state) {
        int redCells = 0, greenCells = 0;
        
        for (const auto& row : state.board) {
            for (const auto& cell : row) {
                if (cell.numberOfOrbs > 0) {
                    if (cell.orbColor == RED) {
                        redCells++;
                    } else {
                        greenCells++;
                    }
                }
            }
        }
        return {redCells, greenCells};
    }
    
    // Helper function to count cells close to explosion
    pair<int, int> countCriticalCells(const State& state) {
        int redCritical = 0, greenCritical = 0;
        int rows = state.board.size();
        int cols = state.board[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                const Cell& cell = state.board[i][j];
                if (cell.numberOfOrbs > 0) {
                    int criticalMass = getCriticalMass(i, j, rows, cols);
                    if (cell.numberOfOrbs >= criticalMass - 1) { // Close to explosion
                        if (cell.orbColor == RED) {
                            redCritical++;
                        } else {
                            greenCritical++;
                        }
                    }
                }
            }
        }
        return {redCritical, greenCritical};
    }
    
    // Helper function to evaluate board control (corner and edge advantage)
    pair<int, int> evaluateBoardControl(const State& state) {
        int redControl = 0, greenControl = 0;
        int rows = state.board.size();
        int cols = state.board[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                const Cell& cell = state.board[i][j];
                if (cell.numberOfOrbs > 0) {
                    int weight = 1;
                    
                    // Corner cells are more valuable (harder to explode)
                    if ((i == 0 || i == rows - 1) && (j == 0 || j == cols - 1)) {
                        weight = 3;
                    }
                    // Edge cells have medium value
                    else if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                        weight = 2;
                    }
                    
                    if (cell.orbColor == RED) {
                        redControl += weight;
                    } else {
                        greenControl += weight;
                    }
                }
            }
        }
        return {redControl, greenControl};
    }

    // Heuristic 1: Simple Orb Count Difference
    int orbCountHeuristic(const State& state, Color maximizingPlayer) {
        auto [redOrbs, greenOrbs] = countOrbs(state);
        
        if (maximizingPlayer == RED) {
            return redOrbs - greenOrbs;
        } else {
            return greenOrbs - redOrbs;
        }
    }
    
    // Heuristic 2: Cell Control (number of cells owned)
    int cellControlHeuristic(const State& state, Color maximizingPlayer) {
        auto [redCells, greenCells] = countCells(state);
        
        if (maximizingPlayer == RED) {
            return redCells - greenCells;
        } else {
            return greenCells - redCells;
        }
    }
    
    // Heuristic 3: Strategic Position (corner and edge control)
    int strategicPositionHeuristic(const State& state, Color maximizingPlayer) {
        auto [redControl, greenControl] = evaluateBoardControl(state);
        
        if (maximizingPlayer == RED) {
            return redControl - greenControl;
        } else {
            return greenControl - redControl;
        }
    }
    
    // Heuristic 4: Critical Mass Advantage (cells close to explosion)
    int criticalMassHeuristic(const State& state, Color maximizingPlayer) {
        auto [redCritical, greenCritical] = countCriticalCells(state);
        
        if (maximizingPlayer == RED) {
            return redCritical - greenCritical;
        } else {
            return greenCritical - redCritical;
        }
    }
    
    // Heuristic 5: Composite Weighted Heuristic
    int compositeHeuristic(const State& state, Color maximizingPlayer) {
        auto [redOrbs, greenOrbs] = countOrbs(state);
        auto [redCells, greenCells] = countCells(state);
        auto [redControl, greenControl] = evaluateBoardControl(state);
        auto [redCritical, greenCritical] = countCriticalCells(state);
        
        int orbDiff = (maximizingPlayer == RED) ? (redOrbs - greenOrbs) : (greenOrbs - redOrbs);
        int cellDiff = (maximizingPlayer == RED) ? (redCells - greenCells) : (greenCells - redCells);
        int controlDiff = (maximizingPlayer == RED) ? (redControl - greenControl) : (greenControl - redControl);
        int criticalDiff = (maximizingPlayer == RED) ? (redCritical - greenCritical) : (greenCritical - redCritical);
        
        // Weighted combination
        return orbDiff * 2 + cellDiff * 5 + controlDiff * 3 + criticalDiff * 4;
    }
    
    public: 

    int evaluate(const State& state, int heuristicType = 5) {
        Color maximizingPlayer = getMaximizingPlayer();

        // Check for terminal states first
        auto [redOrbs, greenOrbs] = countOrbs(state);
        
        // If one player has no orbs, the other wins
        if (redOrbs == 0 && greenOrbs > 0) {
            return (maximizingPlayer == RED) ? -10000 : 10000;
        }
        if (greenOrbs == 0 && redOrbs > 0) {
            return (maximizingPlayer == RED) ? 10000 : -10000;
        }
        
        // Apply selected heuristic
        switch (heuristicType) {
            case 1: return orbCountHeuristic(state, maximizingPlayer);
            case 2: return cellControlHeuristic(state, maximizingPlayer);
            case 3: return strategicPositionHeuristic(state, maximizingPlayer);
            case 4: return criticalMassHeuristic(state, maximizingPlayer);
            case 5: return compositeHeuristic(state, maximizingPlayer);
            default: return compositeHeuristic(state, maximizingPlayer);
        }
    }
};

#endif