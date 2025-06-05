#include <iostream>

#include "util.hpp"
#include "game.hpp"
#include "chainReaction.hpp"

using namespace std;

int main() {
    int rows = 9;
    int cols = 6;
    Player player1 = {HUMAN, RED};
    Player player2 = {HUMAN, GREEN};
    ChainReaction game(rows, cols, player1, player2);
    game.start();
    return 0;
}