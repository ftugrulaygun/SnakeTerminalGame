#include "Leaderboard.h"
#include <iostream>

Leaderboard::~Leaderboard() { clear(); }

void Leaderboard::insertSorted(Snake *s)
{
    // TODO: Insert s so the list stays sorted:
    //   1) higher score first
    //   2) equal scores -> smaller id first ('A' before 'B')
    RankNode* node = new RankNode(s);
    node->next = nullptr;

    if(head == nullptr || s->getScore() > head->snake->getScore() || (s->getScore() == head->snake->getScore() && s->getId() < head->snake->getId()))
    {
        node->next = head;
        head = node;
    }else
    {
        RankNode* current = head;
        while(current->next != nullptr)
        {
            Snake* nextSnake = current->next->snake;

            if(s->getScore() > nextSnake->getScore() || (s->getScore() == nextSnake->getScore() && s->getId() < nextSnake->getId()))
            {
                break;
            }
            current = current->next;
        }
        node->next = current;
        current->next = node;
    }

    size++;
}

bool Leaderboard::remove(Snake *s)
{
    // TODO: Find the node whose snake == s, unlink and delete the NODE.
    // Do NOT delete the snake itself; the Arena owns the snakes.
    // Return false if not found.
    if(head == nullptr)
    {
        return false;
    }
    if(head->snake == s)
    {
        RankNode* toBeDeleted = head;
        head = head->next;
        delete toBeDeleted;
        size--;
        return true; 
    }
    
    RankNode* current = head;
    while(current != nullptr)
    {
        if(current->snake == s)
        {
            RankNode* toBeDeleted = current;
            current = current->next;
            delete toBeDeleted;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

void Leaderboard::reposition(Snake *s)
{
    // TODO: Called after s's score changed. Put s back at its correct place.
    remove(s);
    insertSorted(s);
}

void Leaderboard::clear()
{
    // TODO: Delete all RankNodes (not the snakes).
    RankNode* current = head;

    while(current != nullptr)
    {
        delete current;
        current = current->next;
    }
    head = nullptr;
    size = 0;
}

// Given ready
void Leaderboard::print() const
{
    std::cout << "--- Leaderboard ---" << std::endl;
    if (!head)
    {
        std::cout << "No snakes in the arena." << std::endl;
        return;
    }
    int rank = 1;
    for (RankNode *cur = head; cur; cur = cur->next, ++rank)
    {
        std::cout << rank << ". " << cur->snake->getId()
                  << " score=" << cur->snake->getScore()
                  << " length=" << cur->snake->getLength() << std::endl;
    }
}
