#include "Arena.h"
#include <iostream>
#include <sstream>
#include <cctype>

Arena::Arena() : firstSnake(nullptr), lastSnake(nullptr), tickCount(0) {}

Arena::~Arena()
{
    // TODO: Free all remaining snakes.
    Snake* current = firstSnake;

    while(current != nullptr)
    {
        Snake* next = current->getNext();
        delete current;
        current = next;
    }
    firstSnake = nullptr;
    lastSnake = nullptr;
}

// ---------------------------------------------------------------- helpers (given)

// A valid snake id is a single uppercase letter A-Z.
static bool isSnakeId(const std::string &s)
{
    return s.size() == 1 && s[0] >= 'A' && s[0] <= 'Z';
}

// ---------------------------------------------------------------- player list

Snake *Arena::findSnake(char id) const
{
    // TODO: Return the ALIVE snake with this id, or nullptr.i
    Snake* current = firstSnake;
    while(current != nullptr)
    {
        if(current->getId() == id && current->isAlive() == true)
        {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void Arena::addSnake(Snake *s)
{
    // TODO: Append s to the end of the player list (turn order = spawn order).
    if(firstSnake == nullptr)
    {
        firstSnake = lastSnake = s;
    }else{
        lastSnake->setNext(s);
        lastSnake = s;
    }

}

void Arena::clearSnakes()
{
    // TODO: Delete every snake in the player list, reset the list
    // and clear the leaderboard.

    leaderboard.clear();
    Snake* current = firstSnake;
    
    while(current != nullptr)
    {
        Snake* next = current->getNext();
        delete current;
        current = next;
    }
    firstSnake = nullptr;
    lastSnake = nullptr;
}

void Arena::removeDeadSnakes()
{
    // TODO: Unlink and delete every snake with isAlive() == false.
    Snake* current = firstSnake;
    Snake* previous = nullptr;
    while(current != nullptr)
    {
        Snake* next = current->getNext();
        if(current->isAlive())
        {
            previous = current;
        }else
        {
            if(current == firstSnake)
            {   
                if(current->getNext() == nullptr)
                {
                    delete current;
                    firstSnake = nullptr;
                    lastSnake = nullptr;
                }else{
                    delete current;
                    firstSnake = next; 
                }
            }else if(current == lastSnake)
            {   
                delete current;
                lastSnake = previous;
                previous = lastSnake;
                previous->setNext(nullptr);
            }else
            {
                Snake* next = current->getNext();
                delete current;
                previous->setNext(next);
            }
        }
        current = next;
        
    }
}

void Arena::killSnake(Snake *s)
{
    // TODO:
    //  - Print: std::cout << "Snake " << id << " died with length " << length
    //           << " and score " << score << "." << std::endl;
    //  - Every cell of its body becomes FOOD_CELL on the board.
    //  - Mark it dead and remove it from the leaderboard.
    //  Note: tick() may still be iterating over the player list.
}

// ---------------------------------------------------------------- movement

void Arena::moveSnake(Snake *s)
{
    // TODO: Move s one cell forward. The rules and messages are in Section 4.6 of the assignment.
    // Messages:
    //   "Snake A hit a wall at (nr,nc)."
    //   "Snake A and Snake B collided head-on at (nr,nc)."
    //   "Snake A bit itself at (nr,nc)."
    //   "Snake A bit Snake B at (nr,nc). Snake B lost K segment(s)."
    //   "Snake A ate food at (nr,nc)."
    // The board must always show the head as the uppercase id, body as lowercase.
}

void Arena::tick()
{
    // TODO:
    //  - increase tickCount and print "--- Tick <tickCount> ---"
    //  - every alive snake moves once, in player list order
    //  - snakes that died during this tick must not move
    //  - dead snakes must be removed from the player list before the tick ends
}

// ---------------------------------------------------------------- commands

void Arena::cmdInitBoard(const std::string &args)
{
    // TODO: INIT_BOARD <rows> <cols>   (the input is always well-formed)
    //  Resets everything: snakes, leaderboard, tick counter.
    //  Create the new board with move assignment:  board = Board(r, c);
    //  "Board initialized with <r> rows and <c> columns."
    std::istringstream ss(args);
    int row, col;
    ss >> row >> col;

    board = Board(row, col);

    Snake* current = firstSnake;
    while(current != nullptr)
    {
        Snake* next = current->getNext();
        delete current;
        current = next;
    }
    firstSnake = nullptr;
    leaderboard.clear();
}

// Given ready, as an example for parsing.
// (Checking the format is not required: the input is always well-formed.)
void Arena::cmdWall(const std::string &args)
{
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;
    if (!board.inBounds(r, c) || board.get(r, c) != EMPTY_CELL)
    {
        std::cout << "Error: Cell (" << r << "," << c << ") is not available." << std::endl;
        return;
    }
    board.set(r, c, WALL_CELL);
    std::cout << "Wall placed at (" << r << "," << c << ")." << std::endl;
}

void Arena::cmdFood(const std::string &args)
{
    // TODO: FOOD <r> <c>   (same rules as WALL)
    //  "Error: Cell (r,c) is not available."
    //  "Food placed at (r,c)."
    std::istringstream ss(args);
    int row, col;
    ss >> row >> col;

    if(!board.inBounds(row, col) || board.get(row, col) != EMPTY_CELL )
    {
        std::cout << "Error: Cell (" << row << "," << col << ") is not available." << std::endl;
        return;
    }
    board.set(row, col, FOOD_CELL);
    std::cout << "Food placed at (" << row << "," << col << ")." << std::endl;
}

void Arena::cmdSpawn(const std::string &args)
{
    // TODO: SPAWN <id> <r> <c> <dir> <len>
    //  The head is at (r,c), the body extends in the OPPOSITE direction of <dir>.
    //  alive snake with same id -> "Error: Snake <id> already exists."
    //  any cell out of bounds or not EMPTY_CELL -> "Error: Cannot spawn Snake <id> at (r,c)."
    //  Add to the player list AND to the leaderboard.
    //  "Snake <id> spawned at (r,c) facing <DIR> with length <len>."
}

void Arena::cmdTurn(const std::string &args)
{
    // TODO: TURN <id> <dir>
    //  "Error: Snake <id> not found."
    //  "Error: Snake <id> cannot turn back."
    //  "Snake <id> is now facing <DIR>."
}

void Arena::cmdReverse(const std::string &args)
{
    // TODO: REVERSE <id>
    //  "Error: Snake <id> not found."
    //  Update the board too (old head becomes lowercase, new head uppercase).
    //  "Snake <id> reversed and is now facing <DIR>."
}

void Arena::cmdTick(const std::string &args)
{
    // TODO: TICK [n]   (n is optional, default 1)
}

// Given ready
void Arena::printSnakes() const
{
    std::cout << "--- Snakes ---" << std::endl;
    if (!firstSnake)
    {
        std::cout << "No snakes in the arena." << std::endl;
        return;
    }
    for (Snake *cur = firstSnake; cur; cur = cur->getNext())
    {
        std::cout << cur->getId() << " " << directionToString(cur->getDirection())
                  << " length=" << cur->getLength() << " score=" << cur->getScore()
                  << ": " << cur->getBody() << std::endl;
    }
}

// Given ready
void Arena::processCommand(const std::string &line)
{
    std::istringstream ss(line);
    std::string cmd;
    if (!(ss >> cmd))
        return;
    std::string args;
    std::getline(ss, args);

    if (cmd == "INIT_BOARD")
    {
        cmdInitBoard(args);
        return;
    }

    bool knownBoardCmd = cmd == "WALL" || cmd == "FOOD" || cmd == "SPAWN" || cmd == "TURN" ||
                         cmd == "REVERSE" || cmd == "TICK" || cmd == "PRINT_BOARD" ||
                         cmd == "PRINT_SNAKES" || cmd == "PRINT_LEADERBOARD";
    if (!knownBoardCmd)
    {
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl;
        return;
    }
    if (!board.isInitialized())
    {
        std::cout << "Error: Board not initialized." << std::endl;
        return;
    }

    if (cmd == "WALL")
        cmdWall(args);
    else if (cmd == "FOOD")
        cmdFood(args);
    else if (cmd == "SPAWN")
        cmdSpawn(args);
    else if (cmd == "TURN")
        cmdTurn(args);
    else if (cmd == "REVERSE")
        cmdReverse(args);
    else if (cmd == "TICK")
        cmdTick(args);
    else if (cmd == "PRINT_BOARD")
        board.print();
    else if (cmd == "PRINT_SNAKES")
        printSnakes();
    else if (cmd == "PRINT_LEADERBOARD")
        leaderboard.print();
}
