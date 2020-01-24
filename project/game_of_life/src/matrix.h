#pragma once

#include <iostream>
#include "cell.h"

using cell_matrix = std::vector<std::vector<Cell>>;

class matrix {
public:
    matrix(int rows, int columns);
    void print() const;
    int getRowSize() const;
    int getColumnSize() const;
    bool get(int x, int y) const;
    void set(int x, int y, bool state);
    int getNeighbours(int x, int y) const;
    void mark(int x, int y, bool state);
    void applyMarked();

private:
    bool isUpperLeft(int x, int y) const;
    bool isBottomLeft(int x, int y) const;
    bool isUpperRight(int x, int y) const;
    bool isBottomRight(int x, int y) const;
    bool isUpper(int x, int y) const;
    bool isBottom(int x, int y) const;
    bool isLeft(int x, int y) const;
    bool isRight(int x, int y) const;

    std::unique_ptr<cell_matrix> cell_matrix_;
    int rows_;
    int columns_;
};