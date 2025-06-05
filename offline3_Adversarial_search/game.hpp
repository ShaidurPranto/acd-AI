#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>

using namespace std;

enum PlayMode {
    HUMAN,
    AI
};

enum Color {
    EMPTY,
    RED,
    GREEN
};

struct Player {
    PlayMode mode;
    Color playerColor;
    int depth;
};

struct Cell {
    int numberOfOrbs;
    Color orbColor;
};

struct State {
    vector<vector<Cell>> board;
    Color turn;
};


#endif