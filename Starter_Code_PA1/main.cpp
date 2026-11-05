#include "Arena.h"
#include <iostream>
#include <fstream>
#include <string>

/* You will not submit main.cpp; you can change it freely while testing. */

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./SnakeArena <input_file>\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile)
    {
        std::cerr << "Error: Cannot open input file.\n";
        return 1;
    }

    Arena arena;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        // skip empty lines and comment lines
        if (line.empty() || line[0] == '#')
            continue;
        arena.processCommand(line);
    }
    return 0;
}
