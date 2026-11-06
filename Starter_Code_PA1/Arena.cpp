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
        s->setNext(nullptr);
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
                lastSnake->setNext(nullptr);
            }else
            {
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
    std::cout << "Snake " << s->getId() << " died with length " << s->getLength() << " and score " << s->getScore() << "." << std::endl;

    Segment* current = s->getBody().getHead();
    
    while(current != nullptr)
    {
        Segment* next = current->getNext();
        int row = current->getRow();
        int col = current->getCol();
        
        board.set(row, col, FOOD_CELL);
        current = next;
    }
    s->setAlive(false);
    leaderboard.remove(s);

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
    char lowerId = static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId())));

    Segment* head = s->getBody().getHead();
    int newRow = head->getRow();
    int newCol = head->getCol();
    s->getNextHeadPosition(newRow, newCol);

    Segment* tail = s->getBody().getTail();
    int tailRow = tail->getRow();
    int tailCol = tail->getCol();
    
    if(!board.inBounds(newRow, newCol))
    {
        std::cout << "Snake " << s->getId() << " hit a wall at (" << newRow << "," << newCol << ")." << std::endl;
        killSnake(s);
        return;
    }
    

    bool isOwnTail = (newRow == tailRow && newCol == tailCol);

    char cell = board.get(newRow, newCol);
    
    if(cell == EMPTY_CELL)
    {
        int oldHeadRow = head->getRow();
        int oldHeadCol = head->getCol();

        s->getBody().pushFront(newRow, newCol);
        bool popSituation = s->getBody().popBack(tailRow, tailCol);
        board.set(oldHeadRow, oldHeadCol, static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId()))));

        board.set(tailRow, tailCol, EMPTY_CELL);

        board.set(newRow, newCol, static_cast<char>(std::toupper(static_cast<unsigned char>(s->getId()))));


    }else if(cell == static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId()))))
    {
        if(isOwnTail)
        {
            int oldHeadRow = head->getRow();
            int oldHeadCol = head->getCol();

            s->getBody().pushFront(newRow, newCol);
            bool pop = s->getBody().popBack(tailRow, tailCol);

            board.set(oldHeadRow, oldHeadCol, static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId()))));
            board.set(tailRow, tailCol, EMPTY_CELL);
            board.set(newRow, newCol, static_cast<char>(std::toupper(static_cast<unsigned char>(s->getId()))));
        }else
        {
            std::cout << "Snake " << s->getId() << " bit itself at (" << newRow << "," << newCol << ")." << std::endl; 
            killSnake(s);
            return;
        }

    }else if(cell == FOOD_CELL)
    {
        int oldHeadRow = head->getRow();
        int oldHeadCol = head->getCol();

        s->getBody().pushFront(newRow, newCol);

        board.set(oldHeadRow, oldHeadCol, static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId()))));
        board.set(newRow, newCol, static_cast<char>(std::toupper(static_cast<unsigned char>(s->getId()))));

        s->addScore(1);
        leaderboard.reposition(s);

        std::cout << "Snake " << s->getId() << " ate food at (" << newRow << "," << newCol << ")." << std::endl;

    }else if(cell == WALL_CELL)
    {
        std::cout << "Snake " << s->getId() << " hit a wall at (" << newRow << "," << newCol << ")." << std::endl;
        killSnake(s);
        return;
    }else if('a' <= cell && cell <= 'z' && cell != lowerId)
    {
        char otherId = static_cast<char>(std::toupper(static_cast<unsigned char>(cell)));
        Snake* otherSnake = findSnake(otherId);

        Segment* bittenSegment = otherSnake->getBody().findAt(newRow, newCol);

        int bittenRow = bittenSegment->getRow();
        int bittenCol = bittenSegment->getCol();

        SnakeBody bittenPart = otherSnake->getBody().splitAt(bittenRow, bittenCol);
        int bittenLength = bittenPart.getLength();
        Segment* current = bittenPart.getHead();

        while(current != nullptr)
        {
            Segment* next = current->getNext();
            int row = current->getRow();
            int col = current->getCol();
            
            board.set(row, col, FOOD_CELL);
            current = next;
        }
        int oldHeadRow = head->getRow();
        int oldHeadCol = head->getCol();

        s->getBody().pushFront(newRow, newCol);

        board.set(oldHeadRow, oldHeadCol, static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId()))));
        board.set(newRow, newCol, static_cast<char>(std::toupper(static_cast<unsigned char>(s->getId()))));

        s->addScore(1);
        leaderboard.reposition(s);
        
        std::cout << "Snake " << s->getId() << " bit " << otherSnake->getId() << " at (" << newRow << "," << newCol << ")." << std::endl; 
        std::cout << "Snake " << otherSnake->getId() << " lost " << bittenLength << " segment(s)." << std::endl;
        std::cout << "Snake " << s->getId() << " ate food at (" << newRow << "," << newCol << ")." << std::endl;

    }else if('A' <= cell && cell <= 'Z' && cell != static_cast<char>(std::toupper(static_cast<unsigned char>(lowerId))))
    {
        char otherId = cell;
        Snake* otherSnake = findSnake(otherId);
        int snakeLength = s->getBody().getLength();
        int otherLength = otherSnake->getBody().getLength();

        std::cout << "Snake " << s->getId() << " and Snake "<< otherSnake->getId() << " collided head-on at (" << newRow << "," << newCol << ")." << std::endl;
        if(snakeLength > otherLength)
        {
            int oldHeadRow = s->getBody().getHead()->getRow();
            int oldHeadCol = s->getBody().getHead()->getCol();
            killSnake(otherSnake);
            s->getBody().pushFront(newRow, newCol);
            std::cout << "Snake " << s->getId() << " ate food at ("<< newRow << "," << newCol << ")." << std::endl;
            board.set(oldHeadRow, oldHeadCol, static_cast<char>(std::tolower(static_cast<unsigned char>(s->getId()))));
            board.set(newRow, newCol, static_cast<char>(std::toupper(static_cast<unsigned char>(s->getId()))));

            s->addScore(1);
            leaderboard.reposition(s);
            
        }else if(otherLength > snakeLength)
        {
            killSnake(s);
        }else
        {
            killSnake(s);
            killSnake(otherSnake);
        }
    }
}

void Arena::tick()
{
    // TODO:
    //  - increase tickCount and print "--- Tick <tickCount> ---"
    //  - every alive snake moves once, in player list order
    //  - snakes that died during this tick must not move
    //  - dead snakes must be removed from the player list before the tick ends
    tickCount++;
    std::cout << "--- Tick "<< tickCount << " ---" << std::endl;

    Snake* current = firstSnake;

    while(current != nullptr)
    {   
        if(current->isAlive())
        {
            moveSnake(current);
        }
        current = current->getNext();
    }

    removeDeadSnakes();
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
    lastSnake = nullptr;

    tickCount = 0;
    leaderboard.clear();

    std::cout << "Board initialized with " << row << " rows and " << col << " columns." << std::endl; 
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
    std::istringstream ss(args);
    std::string dirStr;
    int r, c, length;
    char id;

    ss >> id >> r >> c >> dirStr >> length;
    Direction dir;
    parseDirection(dirStr, dir);

    if(findSnake(id) != nullptr)
    {
        std::cout << "Error: Snake " << id << " already exists." << std::endl;
        return;  
    }

    
    for(int i = 0; i < length; i++)
    {
        if(board.inBounds(r - i*rowDelta(dir), c - i*colDelta(dir)) && board.get(r - i*rowDelta(dir), c - i*colDelta(dir)) == EMPTY_CELL)
        {
            continue;
        }else
        {
            std::cout << "Error: Cannot spawn Snake " << id << " at (" << r << "," << c << ")." << std::endl;
            return;
        }
    } 

    Snake* spawnedSnake = new Snake(id, dir);
    for(int i = 0; i < length; i++)
    {
        int row = r - i*rowDelta(dir);
        int col = c - i*colDelta(dir);
        
        spawnedSnake->getBody().pushBack(row, col);
    }

    Segment* current = spawnedSnake->getBody().getHead();
    
    board.set(r, c, static_cast<char>(std::toupper(static_cast<unsigned char>(id))));

    current = current->getNext();
    int i = 1;

    while(current != nullptr)
    {      
        
        int rowD = r - i*rowDelta(dir);
        int colD = c - i*colDelta(dir);
        board.set(rowD, colD, static_cast<char>(std::tolower(static_cast<unsigned char>(id))));
        current = current->getNext();

        i++;
    }

    addSnake(spawnedSnake);
    leaderboard.insertSorted(spawnedSnake);

    std::cout << "Snake " << id << " spawned at (" << r << "," << c << ") facing " << directionToString(dir) << " with length " << length << "." << std::endl;
}

void Arena::cmdTurn(const std::string &args)
{
    // TODO: TURN <id> <dir>
    //  "Error: Snake <id> not found."
    //  "Error: Snake <id> cannot turn back."
    //  "Snake <id> is now facing <DIR>."

    std::istringstream ss(args);
    std::string dirStr;
    char id;

    ss >> id >> dirStr;

    Direction dir;

    bool parseSit = parseDirection(dirStr, dir);

    Snake* currentSnake = findSnake(id);

    if(currentSnake == nullptr)
    {
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }

    bool turnSit = currentSnake->turn(dir);
    if(!turnSit)
    {
        std::cout << "Error: Snake " << id << " cannot turn back." << std::endl;
        return;
    }
    else
    {
        std::cout << "Snake " << id << " is now facing " << directionToString(dir) << "." << std::endl;
    }
}

void Arena::cmdReverse(const std::string &args)
{
    // TODO: REVERSE <id>
    //  "Error: Snake <id> not found."
    //  Update the board too (old head becomes lowercase, new head uppercase).
    //  "Snake <id> reversed and is now facing <DIR>."

    std::istringstream ss(args);
    char id;
    ss >> id;
    if(findSnake(id) == nullptr){
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }
    
    Snake* currentSnake = findSnake(id);

    if(currentSnake->getBody().getLength() == 1)
    {
        currentSnake->reverse();
        std::cout << "Snake " << id << " reversed and is now facing " << directionToString(currentSnake->getDirection()) << std::endl;
        return;
    }
    

    char headID = static_cast<char>(std::toupper(static_cast<unsigned char>(currentSnake->getId())));
    char tailID = static_cast<char>(std::tolower(static_cast<unsigned char>(currentSnake->getId()))); 

    int headRow = currentSnake->getBody().getHead()->getRow();
    int headCol = currentSnake->getBody().getHead()->getCol();

    int tailRow = currentSnake->getBody().getTail()->getRow();
    int tailCol = currentSnake->getBody().getTail()->getCol();

    currentSnake->reverse();

    board.set(headRow, headCol, tailID);
    board.set(tailRow, tailCol, headID);

    std::cout << "Snake " << id << " reversed and is now facing " << directionToString(currentSnake->getDirection()) << std::endl;

}

void Arena::cmdTick(const std::string &args)
{
    // TODO: TICK [n]   (n is optional, default 1)
    int n = 1;
    std::istringstream ss(args);
    ss >> n;

    for(int i = 0; i < n; i++)
    {
        tick();
    }
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
    for (Snake *cur = firstSnake; cur != nullptr; cur = cur->getNext())
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
