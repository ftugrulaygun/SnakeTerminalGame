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
        node->next = current->next;
        current->next = node;
    }

    size++;
}

bool Leaderboard::remove(Snake *s)
{
    // TODO: Find the node whose snake == s, unlink and delete the NODE.
    // Do NOT delete the snake itself; the Arena owns the snakes.
    // Return false if not found.

    RankNode* previous = nullptr;
    RankNode* current = head;
    
    while(current != nullptr)
    {
        if(current->snake == s)
        {
            if(previous == nullptr)
            {
                head = current->next;
            }
                
            else
            {
                previous->next = current->next;
            }

            delete current;
            size--;
            return true;
        }
        previous = current;
        current = current->next;
        
    }
    return false;

    
}

void Leaderboard::reposition(Snake *s)
{
    // TODO: Called after s's score changed. Put s back at its correct place.
    if(remove(s))
    {
        insertSorted(s);
    }
}

void Leaderboard::clear()
{
    // TODO: Delete all RankNodes (not the snakes).
    RankNode* current = head;

    while(current != nullptr)
    {   
        RankNode* next = current->next;
        delete current;
        current = next;
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
