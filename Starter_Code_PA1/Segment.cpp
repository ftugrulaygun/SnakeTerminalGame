#include "Segment.h"

Segment::Segment(int _row, int _col)
    : row(_row), col(_col), prev(nullptr), next(nullptr) {}

int Segment::getRow() const { return row; }
int Segment::getCol() const { return col; }
Segment *Segment::getPrev() const { return prev; }
Segment *Segment::getNext() const { return next; }
void Segment::setPrev(Segment *p) { prev = p; }
void Segment::setNext(Segment *n) { next = n; }

std::ostream &operator<<(std::ostream &os, const Segment &s)
{
    os << "(" << s.row << "," << s.col << ")";
    return os;
}
