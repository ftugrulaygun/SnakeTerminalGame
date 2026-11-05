#ifndef ARENA_H
#define ARENA_H

#include <string>
#include "Board.h"
#include "Snake.h"
#include "Leaderboard.h"

const int MAX_BOARD_SIZE = 100;

// Central controller: owns the board, the snakes and the leaderboard,
// and executes commands read from the input file.
class Arena
{
private:
    Board board;
    Snake *firstSnake; // Player list (turn order = spawn order)
    Snake *lastSnake;
    Leaderboard leaderboard;
    int tickCount;

    Snake *findSnake(char id) const;
    void addSnake(Snake *s);
    void clearSnakes();
    void removeDeadSnakes();
    void killSnake(Snake *s);
    void moveSnake(Snake *s);
    void tick();

    // Command handlers
    void cmdInitBoard(const std::string &args);
    void cmdWall(const std::string &args);
    void cmdFood(const std::string &args);
    void cmdSpawn(const std::string &args);
    void cmdTurn(const std::string &args);
    void cmdReverse(const std::string &args);
    void cmdTick(const std::string &args);
    void printSnakes() const;

public:
    Arena();
    ~Arena();

    Arena(const Arena &) = delete;
    Arena &operator=(const Arena &) = delete;

    void processCommand(const std::string &line);
};

#endif
