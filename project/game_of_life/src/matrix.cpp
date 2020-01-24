#include <vector>
#include "cell.h"
#include "matrix.h"

matrix::matrix(int rows, int columns): rows_(rows), columns_(columns){
    std::vector<std::vector<Cell>> matrix;
    std::generate_n(std::back_inserter(matrix), columns, [&rows](){
        std::vector<Cell> inner;
        std::generate_n(std::back_inserter(inner), rows, [](){
            return Cell(false);
        });
        return inner;
    });

    cell_matrix_ =  std::make_unique<cell_matrix>(std::move(matrix));
}

void matrix::print() const {
    std::cout << std::endl << std::endl;
    for (std::vector<Cell>& column: *cell_matrix_) { // columns in matrix
        for (Cell& cell: column) {                   // cell in column
            std::cout << cell.getState() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool matrix::get(int x, int y) const {
    return (*cell_matrix_)[x][y].getState();
}

void matrix::set(int x, int y, bool state) {
    (*cell_matrix_)[x][y].setState(state);
}

void matrix::mark(int x, int y, bool state) {
    (*cell_matrix_)[x][y].markForStateChange(state);
}

void matrix::applyMarked() {
    for (std::vector<Cell>& column: *cell_matrix_) {
        for (Cell& cell: column) {
            cell.applyStateChange();
        }
        std::cout << std::endl;
    }
}

int matrix::getRowSize() const {
    return rows_;
}

int matrix::getColumnSize() const {
    return columns_;
}

bool matrix::isUpperLeft(int x, int y) const {
    return isUpper(x ,y) && isLeft(x ,y);
}

bool matrix::isBottomLeft(int x, int y) const {
    return isBottom(x ,y) && isLeft(x ,y);
}

bool matrix::isUpperRight(int x, int y) const {
    return isUpper(x ,y) && isRight(x ,y);
}

bool matrix::isBottomRight(int x, int y) const {
    return isBottom(x ,y) && isRight(x ,y);
}

bool matrix::isUpper(int x, int y) const {
    return (y == 0);
}

bool matrix::isBottom(int x, int y) const {
    return (y == rows_-1);
}

bool matrix::isLeft(int x, int y) const {
    return (x == 0);
}

bool matrix::isRight(int x, int y) const {
    return  (x == columns_-1);
}

int matrix::getNeighbours(int x, int y) const {
    auto n = 0;

    if (this->isUpperLeft(x,y)) {
        n =     get(columns_-1, rows_-1) +  get(x, rows_-1) +       get(x+1, rows_-1) +
                get(columns_-1, y) +                                get(x+1, y) +
                get(columns_-1, y+1) +      get(x, y+1) +           get(x+1, y+1);
    } else if (this->isUpperRight(x,y)) {
        n =     get(x-1, rows_-1) +         get(x-0, rows_-1) +     get(0, rows_-1) +
                get(x-1, y) +                                       get(0, y) +
                get(x-1, y+1) +             get(x-0, y+1) +         get(0, y+1);
    } else if (this->isBottomLeft(x,y)) {
        n =     get(columns_-1, y-1) +      get(x-0, y-1) +         get(x+1, y-1) +
                get(columns_-1, y-0) +                              get(x+1, y-0) +
                get(columns_-1, 0) +        get(x-0, 0) +           get(x+1, 0);
    } else if (this->isBottomRight(x,y)) {
        n =     get(x-1, y-1) +             get(x-0, y-1) +         get(0, y-1) +
                get(x-1, y-0) +                                     get(0, y-0) +
                get(x-1, 0) +               get(x-0, 0) +           get(0, 0);
    } else if (this->isLeft(x,y)) {
        n =     get(columns_-1, y-1) +      get(x-0, y-1) +         get(x+1, y-1) +
                get(columns_-1, y-0) +                              get(x+1, y-0) +
                get(columns_-1, y+1) +      get(x-0, y+1) +         get(x+1, y+1);
    } else if (this->isRight(x,y)) {
        n =     get(x-1, y-1) +             get(x-0, y-1) +         get(0, y-1) +
                get(x-1, y-0) +                                     get(0, y-0) +
                get(x-1, y+1) +             get(x-0, y+1) +         get(0, y+1);
    } else if (this->isBottom(x,y)) {
        n =     get(x-1, y-1) +             get(x-0, y-1) +         get(x+1, y-1) +
                get(x-1, y-0) +                                     get(x+1, y-0) +
                get(x-1, 0) +               get(x-0, 0) +           get(x+1, 0);
    } else if (this->isUpper(x,y)) {
        n =     get(x-1, rows_-1) +         get(x-0, rows_-1) +     get(x+1, rows_-1) +
                get(x-1, y-0) +                                     get(x+1, y-0) +
                get(x-1, y+1) +             get(x-0, y+1) +         get(x+1, y+1);
    } else {
        n =     get(x-1, y-1) +             get(x-0, y-1) +         get(x+1, y-1) +
                get(x-1, y-0) +                                     get(x+1, y-0) +
                get(x-1, y+1) +             get(x-0, y+1) +         get(x+1, y+1);
    }

    return n;
}
