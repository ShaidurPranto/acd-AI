#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>

#define UI 0

#define TIME_DELAY_AMONG_TWO_AI_MOVES 1000 // in milliseconds
#define TIME_DELAY_AMONG_TWO_CHAIN_REACTIONS 1000 // in milliseconds


using namespace std;

enum PlayMode
{
    HUMAN,
    AI
};

enum Color
{
    EMPTY,
    RED,
    GREEN
};

struct Player
{
    PlayMode mode;
    Color playerColor;
    int depth;
    int htype;
};

struct Cell
{
    int numberOfOrbs;
    Color orbColor;
};

struct State
{
    vector<vector<Cell>> board;
    Color turn;
};

struct Move
{
    int row;
    int col;
    int value;

    Move()
    {
        this->row = this->col = -1;
        this->value = 0;
    }
    Move(int value)
    {
        this->value = value;
        this->col = this->row = -1;
    }
    Move(int row, int col)
    {
        this->row = row;
        this->col = col;
        this->value = 0;
    }
    Move(int row, int col, int value)
    {
        this->value = value;
        this->row = row;
        this->col = col;
    }
};

// assuming RED is the maximizing player
Color getMaximizingPlayer() {
    return RED;
}

// debugging purpose
bool debug(){
    return false;
}

#endif