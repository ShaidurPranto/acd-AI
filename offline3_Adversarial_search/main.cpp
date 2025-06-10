#include <iostream>

#include "game.hpp"
#include "chainReaction.hpp"

using namespace std;

int main() {
    int rows = 3;
    int cols = 3;
    Player player1 = { AI, RED, 1, 2 }; // mode, color, depth, heuristic_type(1-5) 
    Player player2 = { AI, GREEN, 1, 5 };  // depth , heuristic_type is optional for HUMAN
    ChainReaction game(rows, cols, player1, player2, "game_state.txt");
    game.start();
    return 0;
}

