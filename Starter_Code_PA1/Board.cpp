#include "Board.h"
#include <iostream>

Board::Board() : rows(0), cols(0), cells(nullptr) {}

Board::Board(int _rows, int _cols) : rows(_rows), cols(_cols), cells(nullptr)
{
    // TODO: Allocate a rows x cols matrix and fill every cell with EMPTY_CELL.
    cells = new char*[rows];
    for(int i = 0; i<rows; i++)
    {
        cells[i] = new char[cols];
        for(int j = 0; j < cols; j++)
        {
            cells[i][j] = EMPTY_CELL;
        }
    }
}

Board::~Board()
{
    // TODO: Free everything this board owns.
    if(cells != nullptr)
    {
        for(int i = 0; i < rows; i++)
        {
            delete[] cells[i];    
        }

        delete[] cells;
    }
}

Board::Board(Board &&other) noexcept
    : rows(0), cols(0), cells(nullptr)
{
    // TODO: Take over other's matrix, leave other as an empty board (0, 0, nullptr).
    rows = other.rows;
    cols = other.cols;
    cells = other.cells;
    
    other.rows = 0;
    other.cols = 0;
    other.cells = nullptr;
}

Board &Board::operator=(Board &&other) noexcept
{
    // TODO: Replace this board's matrix with other's (no leaks).
    // 'other' must be left as an empty board (0, 0, nullptr).
    if(this != &other)
    {
        if(cells != nullptr)
        {
            for(int i = 0; i<rows; i++)
            {
                delete[] cells[i];
            }
            delete[] cells;
        }
        rows = other.rows;
        cols = other.cols;
        cells = other.cells;

        other.rows = 0;
        other.cols = 0;
        other.cells = nullptr;
    }
    return *this;
}

bool Board::inBounds(int r, int c) const
{
    // TODO: true if (r,c) is inside the board.
    return (r >= 0 && r < rows && c >= 0 && c < cols);  
}

char Board::get(int r, int c) const { return cells[r][c]; }

void Board::set(int r, int c, char ch) { cells[r][c] = ch; }

// Given ready
void Board::print() const
{
    std::cout << "--- Board " << rows << "x" << cols << " ---" << std::endl;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            std::cout << cells[i][j];
        std::cout << std::endl;
    }
}
