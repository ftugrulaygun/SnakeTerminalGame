#ifndef SEGMENT_H
#define SEGMENT_H

#include <ostream>

// A single body part of a snake: a node of the SnakeBody doubly linked list.
class Segment
{
private:
    int row;
    int col;

    Segment *prev; // Towards the head
    Segment *next; // Towards the tail

public:
    Segment(int _row, int _col);

    int getRow() const;
    int getCol() const;
    Segment *getPrev() const;
    Segment *getNext() const;
    void setPrev(Segment *p);
    void setNext(Segment *n);

    // Prints as (row,col)
    friend std::ostream &operator<<(std::ostream &os, const Segment &s);

    friend class SnakeBody;
};

#endif
