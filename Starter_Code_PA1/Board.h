#ifndef BOARD_H
#define BOARD_H

const char EMPTY_CELL = '.';
const char WALL_CELL = '#';
const char FOOD_CELL = '*';

// The arena grid: a dynamically allocated 2D char matrix.
class Board
{
private:
    int rows;
    int cols;
    char **cells;

public:
    Board();                        // Uninitialized board: 0x0, cells = nullptr
    Board(int _rows, int _cols);    // Allocates rows x cols cells, all EMPTY_CELL
    ~Board();

    Board(const Board &) = delete;
    Board &operator=(const Board &) = delete;

    Board(Board &&other) noexcept;
    Board &operator=(Board &&other) noexcept;

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    bool isInitialized() const { return cells != nullptr; }
    bool inBounds(int r, int c) const;
    char get(int r, int c) const;
    void set(int r, int c, char ch);

    void print() const;
};

#endif
