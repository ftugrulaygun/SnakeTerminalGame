#include "SnakeBody.h"

SnakeBody::~SnakeBody() { clear(); }

SnakeBody::SnakeBody(SnakeBody &&other) noexcept
    : head(nullptr), tail(nullptr), length(0)
{
    // TODO: Take over other's segments. 'other' must be left empty but valid.
    head = other.head;
    tail = other.tail;
    length = other.length;

    other.head = nullptr;
    other.tail = nullptr;
    other.length = 0;
}

SnakeBody &SnakeBody::operator=(SnakeBody &&other) noexcept
{
    // TODO: Same as the move constructor, but this body may already own segments
    // (they must not leak).
    if(this != &other)
    {
        clear();

        head = other.head;
        tail = other.tail;
        length = other.length;

        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
    }
    return *this;
}

bool SnakeBody::isEmpty() const
{
    // TODO: true if the body has no segments.
    return head == nullptr;
}

void SnakeBody::pushFront(int row, int col)
{
    // TODO: Add a new segment at (row,col) as the new head.
    Segment* newSegment = new Segment(row, col);
    
    if(isEmpty())
    {
        head = tail = newSegment;
    }else{
        newSegment->setNext(head);
        head->setPrev(newSegment);
        head = newSegment;
    }
    length++;
}

void SnakeBody::pushBack(int row, int col)
{
    // TODO: Add a new segment at (row,col) as the new tail.
    Segment* newSegment = new Segment(row, col);
    if(isEmpty())
    {
        head = tail = newSegment;
    } else
    {
        tail->setNext(newSegment);
        newSegment->setPrev(tail);
        tail = newSegment;
    }
}

bool SnakeBody::popBack(int &row, int &col)
{
    // TODO: Remove the tail segment, write its coordinates into row/col and delete it.
    // Return false if the body is empty.
    if(isEmpty())
    {
        return false;
    }
    row = tail->getRow();
    col = tail->getCol();

    Segment* toBeDeleted = tail;

    if(head == tail)
    {
        head = tail = nullptr;
    }else
    {
        tail = tail->getPrev();
        tail->setNext(nullptr);
    }

    delete toBeDeleted;
    length--;

    return true;
}

void SnakeBody::reverse()
{
    // TODO: Reverse the list in place (do not create or delete segments).
    Segment* current = head;
    Segment* temp = nullptr;

    while(current != nullptr)
    {   
        temp = current->getNext();
        current->setNext(current->getPrev());
        current->setPrev(temp);

        current = temp;
    }

    temp = head;
    head = tail;
    tail = temp;
}

Segment *SnakeBody::findAt(int row, int col) const
{
    // TODO: Return the segment at (row,col), or nullptr if there is none.
    Segment* current = head;
    while(current != nullptr)
    {   
        if(current->getRow() == row && current->getCol() == col)
        {
            return current;
        }
        current = current->getNext();
    }

    return nullptr;
}

SnakeBody SnakeBody::splitAt(int row, int col)
{
    SnakeBody cut; // returned by value -> moved, not copied

    // TODO: The segment at (row,col) and every segment after it (towards the tail)
    // are moved into 'cut'; this body keeps the segments before it.
    // If there is no segment at (row,col), return an empty body.

    Segment* target = findAt(row, col);

    if(target == nullptr)
    {
        return cut;
    }

    int cutLength = 0;
    for(Segment* current = target; current != nullptr; current = current->getNext())
    {
        cutLength++;
    }

    cut.head = target;
    cut.tail = tail;
    cut.length = cutLength;

    Segment* otherTail = target->getPrev();

    if(otherTail == nullptr)
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
    } else
    {
        otherTail->setNext(nullptr);
        target->setPrev(nullptr);

        tail = otherTail;
        length -= cutLength;
    }

    return cut;
}

void SnakeBody::clear()
{
    // TODO: Delete every segment. The body must be empty and usable afterwards.
    Segment* current = head;
    while(current != nullptr)
    {
        Segment* next = current->getNext();
        delete current;
        current = next;
    }
    head = tail = nullptr;
    length = 0;
}

// Given ready
std::ostream &operator<<(std::ostream &os, const SnakeBody &b)
{
    for (Segment *cur = b.head; cur; cur = cur->getNext())
    {
        os << *cur;
        if (cur->getNext())
            os << " ";
    }
    return os;
}
