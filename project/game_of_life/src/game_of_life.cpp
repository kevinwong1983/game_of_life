#include <iostream>
#include <algorithm>

#include "game_of_life.h"
#include "rules.h"

game_of_life::game_of_life(int rows, int columns):matrix_(std::make_shared<matrix>(rows, columns)) {
    rules::populate(matrix_);
};

void game_of_life::PublishHandler() {
    std::cout << "handle" << std::endl;
    rules::apply(matrix_);
    matrix_->print();
}