#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "Snake.h"

// Node of the leaderboard list. It does NOT own the snake.
struct RankNode
{
    Snake *snake;
    RankNode *next;
    RankNode(Snake *s) : snake(s), next(nullptr) {}
};

// Singly linked list of RankNodes, always kept sorted:
//   higher score first; equal scores -> smaller id first ('A' before 'B').
class Leaderboard
{
private:
    RankNode *head;
    int size;

public:
    Leaderboard() : head(nullptr), size(0) {}
    ~Leaderboard();

    Leaderboard(const Leaderboard &) = delete;
    Leaderboard &operator=(const Leaderboard &) = delete;

    void insertSorted(Snake *s);
    bool remove(Snake *s);     // Removes the node of s (does not delete s)
    void reposition(Snake *s); // Call after s's score changes
    void clear();              // Deletes all RankNodes (not the snakes)
    RankNode *getHead() const { return head; } // first (best ranked) node
    bool isEmpty() const { return head == nullptr; }
    int getSize() const { return size; }

    void print() const;
};

#endif
