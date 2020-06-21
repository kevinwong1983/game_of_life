#include <vector>
#include "matrix.h"
#include "cell_factory.h"

Matrix::Matrix(int rows, int columns, std::shared_ptr<StatePublisher> state_dispatch, const std::string& cell_type) : rows_(rows),
                                                                                        columns_(columns) {
    std::vector<std::vector<std::unique_ptr<Cell>>> Matrix;
    std::generate_n(std::back_inserter(Matrix), columns, [&cell_type, state_dispatch, y = 0, &rows]()mutable {
        std::vector<std::unique_ptr<Cell>> inner;
        std::generate_n(std::back_inserter(inner), rows, [&cell_type, state_dispatch, x = 0, &y]()mutable {
            auto c = CellFactory::make(cell_type, x, y, state_dispatch);
            x++;
            return c;
        });
        y++;
        return inner;
    });

    cell_matrix_ = std::make_unique<cell_matrix>(std::move(Matrix));
}

void Matrix::print() const {
    std::cout << std::endl << std::endl;
    for (auto &column: *cell_matrix_) { // columns in matrix
        for (auto &cell: column) {                   // cell in column
            std::cout << cell->get_state() << "[" << cell->get_x() << "," << cell->get_y() << "]" << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Matrix::get(int x, int y) const {
    auto rv = (*cell_matrix_)[x][y]->get_state();
    return rv;
}

void Matrix::set(int x, int y, bool state) {
    (*cell_matrix_)[x][y]->set_state(state);
}

void Matrix::mark(int x, int y, bool state) {
    (*cell_matrix_)[x][y]->mark_state(state);
}

void Matrix::applyMarked() {
    for (auto &column: *cell_matrix_) {
        for (auto &cell: column) {
            cell->update();
        }
        std::cout << std::endl;
    }
}

void Matrix::subscribeToCell(int x, int y, std::shared_ptr<StatePublisherAbstract> on_state_set) {
    (*cell_matrix_)[x][y]->subscribe(on_state_set);
}

int Matrix::getRowSize() const {
    return rows_;
}

int Matrix::getColumnSize() const {
    return columns_;
}

bool Matrix::isUpperLeft(int x, int y) const {
    return isUpper(x, y) && isLeft(x, y);
}

bool Matrix::isBottomLeft(int x, int y) const {
    return isBottom(x, y) && isLeft(x, y);
}

bool Matrix::isUpperRight(int x, int y) const {
    return isUpper(x, y) && isRight(x, y);
}

bool Matrix::isBottomRight(int x, int y) const {
    return isBottom(x, y) && isRight(x, y);
}

bool Matrix::isUpper(int x, int y) const {
    return (y == 0);
}

bool Matrix::isBottom(int x, int y) const {
    return (y == rows_ - 1);
}

bool Matrix::isLeft(int x, int y) const {
    return (x == 0);
}

bool Matrix::isRight(int x, int y) const {
    return (x == columns_ - 1);
}

int Matrix::getNeighbours(int x, int y) const {
    auto n = 0;

    if (this->isUpperLeft(x, y)) {
        n = get(columns_ - 1, rows_ - 1) + get(x, rows_ - 1) + get(x + 1, rows_ - 1) +
            get(columns_ - 1, y) + get(x + 1, y) +
            get(columns_ - 1, y + 1) + get(x, y + 1) + get(x + 1, y + 1);
    } else if (this->isUpperRight(x, y)) {
        n = get(x - 1, rows_ - 1) + get(x - 0, rows_ - 1) + get(0, rows_ - 1) +
            get(x - 1, y) + get(0, y) +
            get(x - 1, y + 1) + get(x - 0, y + 1) + get(0, y + 1);
    } else if (this->isBottomLeft(x, y)) {
        n = get(columns_ - 1, y - 1) + get(x - 0, y - 1) + get(x + 1, y - 1) +
            get(columns_ - 1, y - 0) + get(x + 1, y - 0) +
            get(columns_ - 1, 0) + get(x - 0, 0) + get(x + 1, 0);
    } else if (this->isBottomRight(x, y)) {
        n = get(x - 1, y - 1) + get(x - 0, y - 1) + get(0, y - 1) +
            get(x - 1, y - 0) + get(0, y - 0) +
            get(x - 1, 0) + get(x - 0, 0) + get(0, 0);
    } else if (this->isLeft(x, y)) {
        n = get(columns_ - 1, y - 1) + get(x - 0, y - 1) + get(x + 1, y - 1) +
            get(columns_ - 1, y - 0) + get(x + 1, y - 0) +
            get(columns_ - 1, y + 1) + get(x - 0, y + 1) + get(x + 1, y + 1);
    } else if (this->isRight(x, y)) {
        n = get(x - 1, y - 1) + get(x - 0, y - 1) + get(0, y - 1) +
            get(x - 1, y - 0) + get(0, y - 0) +
            get(x - 1, y + 1) + get(x - 0, y + 1) + get(0, y + 1);
    } else if (this->isBottom(x, y)) {
        n = get(x - 1, y - 1) + get(x - 0, y - 1) + get(x + 1, y - 1) +
            get(x - 1, y - 0) + get(x + 1, y - 0) +
            get(x - 1, 0) + get(x - 0, 0) + get(x + 1, 0);
    } else if (this->isUpper(x, y)) {
        n = get(x - 1, rows_ - 1) + get(x - 0, rows_ - 1) + get(x + 1, rows_ - 1) +
            get(x - 1, y - 0) + get(x + 1, y - 0) +
            get(x - 1, y + 1) + get(x - 0, y + 1) + get(x + 1, y + 1);
    } else {
        n = get(x - 1, y - 1) + get(x - 0, y - 1) + get(x + 1, y - 1) +
            get(x - 1, y - 0) + get(x + 1, y - 0) +
            get(x - 1, y + 1) + get(x - 0, y + 1) + get(x + 1, y + 1);
    }

    return n;
}
