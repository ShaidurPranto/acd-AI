#include <iostream>

#include "2105177_game.hpp"
#include "2105177_chainReaction.hpp"

using namespace std;

int main() {
    int rows = 9;
    int cols = 6;
    Player player1 = { AI, RED, 1, 4 }; // mode, color, depth, heuristic_type(1-5) 
    Player player2 = { AI, GREEN, 1, 5 };  // depth , heuristic_type is optional for HUMAN
    ChainReaction game(rows, cols, player1, player2, "game_state.txt");
    game.start();
    return 0;
}

