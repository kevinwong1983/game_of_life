#include <iostream>
#include <algorithm>

#include "game_of_life.h"
#include "matrix.h"
#include "rules.h"

GameOfLife::GameOfLife(int rows, int columns, std::shared_ptr<StateDispatcher> state_dispatch) {
    matrix_ = std::make_shared<Matrix>(rows, columns, state_dispatch);
    Rules::populate(matrix_);
    matrix_->print();
};

void GameOfLife::on_publish() {
    Rules::apply(matrix_);
    matrix_->print();
}