#ifndef SNAKEBODY_H
#define SNAKEBODY_H

#include "Segment.h"
#include <ostream>

// Doubly linked list of Segments. head = snake's head, tail = snake's last segment.
class SnakeBody
{
private:
    Segment *head;
    Segment *tail;
    int length;

public:
    SnakeBody() : head(nullptr), tail(nullptr), length(0) {}
    ~SnakeBody();

    // Copying would make two bodies share the same segments (double delete).
    SnakeBody(const SnakeBody &) = delete;
    SnakeBody &operator=(const SnakeBody &) = delete;

    // Moving transfers ownership of the segments.
    SnakeBody(SnakeBody &&other) noexcept;
    SnakeBody &operator=(SnakeBody &&other) noexcept;

    Segment *getHead() const { return head; }
    Segment *getTail() const { return tail; }
    int getLength() const { return length; }
    bool isEmpty() const;

    // Adds a new segment before the current head (it becomes the new head)
    void pushFront(int row, int col);

    // Adds a new segment after the current tail (it becomes the new tail)
    void pushBack(int row, int col);

    // Removes and deletes the tail segment. Writes its coordinates to row/col.
    // Returns false if the body is empty.
    bool popBack(int &row, int &col);

    // Reverses the list in place: head becomes tail, tail becomes head.
    void reverse();

    // Returns the segment at (row,col) or nullptr.
    Segment *findAt(int row, int col) const;

    // Splits the body at (row,col): that segment and everything after it
    // (towards the tail) are moved into the returned SnakeBody.
    // Returns an empty body if no such segment exists.
    SnakeBody splitAt(int row, int col);

    void clear();

    // Prints segments from head to tail: (r,c) (r,c) ...
    friend std::ostream &operator<<(std::ostream &os, const SnakeBody &b);
};

#endif
