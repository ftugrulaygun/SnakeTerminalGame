#include "Enums.h"
#include <algorithm>

/* Enum operations
 * You should not need to change anything in this file */

std::string directionToString(Direction d)
{
    switch (d)
    {
    case Direction::UP:
        return "UP";
    case Direction::DOWN:
        return "DOWN";
    case Direction::LEFT:
        return "LEFT";
    case Direction::RIGHT:
        return "RIGHT";
    }
    return "UP";
}

bool parseDirection(const std::string &s, Direction &out)
{
    std::string up = toUpper(s);
    if (up == "UP")
        out = Direction::UP;
    else if (up == "DOWN")
        out = Direction::DOWN;
    else if (up == "LEFT")
        out = Direction::LEFT;
    else if (up == "RIGHT")
        out = Direction::RIGHT;
    else
        return false;
    return true;
}

Direction oppositeDirection(Direction d)
{
    switch (d)
    {
    case Direction::UP:
        return Direction::DOWN;
    case Direction::DOWN:
        return Direction::UP;
    case Direction::LEFT:
        return Direction::RIGHT;
    case Direction::RIGHT:
        return Direction::LEFT;
    }
    return Direction::UP;
}

int rowDelta(Direction d)
{
    if (d == Direction::UP)
        return -1;
    if (d == Direction::DOWN)
        return 1;
    return 0;
}

int colDelta(Direction d)
{
    if (d == Direction::LEFT)
        return -1;
    if (d == Direction::RIGHT)
        return 1;
    return 0;
}

std::string toUpper(const std::string &s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::toupper);
    return r;
}
