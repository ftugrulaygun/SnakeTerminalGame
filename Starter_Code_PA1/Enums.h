#ifndef ENUMS_H
#define ENUMS_H

#include <string>

/** Direction enum and its helper functions.
 *  All functions in this header are given ready in Enums.cpp
 */

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

std::string directionToString(Direction d);
// Returns false if the string is not a valid direction
bool parseDirection(const std::string &s, Direction &out);
Direction oppositeDirection(Direction d);
int rowDelta(Direction d); // UP: -1, DOWN: +1, others 0
int colDelta(Direction d); // LEFT: -1, RIGHT: +1, others 0
std::string toUpper(const std::string &s);

#endif
