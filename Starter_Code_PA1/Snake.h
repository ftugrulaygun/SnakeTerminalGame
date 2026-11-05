#ifndef SNAKE_H
#define SNAKE_H

#include "SnakeBody.h"
#include "Enums.h"

class Snake
{
private:
    char id; // 'A'..'Z'
    Direction direction;
    int score;
    bool alive;
    SnakeBody body;
    Snake *next; // Next snake in the arena's player list (turn order)

public:
    Snake(char _id, Direction _direction);

    char getId() const { return id; }
    Direction getDirection() const { return direction; }
    int getScore() const { return score; }
    bool isAlive() const { return alive; }
    int getLength() const { return body.getLength(); }
    SnakeBody &getBody() { return body; }
    const SnakeBody &getBody() const { return body; }
    Snake *getNext() const { return next; }
    void setNext(Snake *n) { next = n; }
    void setAlive(bool a) { alive = a; }
    void addScore(int s) { score += s; }

    // Changes direction. Returns false (and changes nothing) if moving in
    // newDir would put the head onto the second segment (the "neck").
    bool turn(Direction newDir);

    // Reverses the body. New direction: from the second segment towards the
    // new head. If length is 1, the direction simply becomes the opposite.
    void reverse();

    // Computes the cell the head will move into on the next step.
    void getNextHeadPosition(int &row, int &col) const;
};

#endif
