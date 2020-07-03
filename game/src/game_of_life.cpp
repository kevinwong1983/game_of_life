#include <iostream>
#include <algorithm>

#include "game_of_life.h"
#include "matrix.h"
#include "rules.h"
#include "cell_factory.h"

GameOfLife::GameOfLife(int rows, int columns, std::shared_ptr<StatePublisher> state_dispatch) : config_(
        std::make_shared<Configuration>()) {
    config_->rows_ = rows;
    config_->columns_ = columns;
    state_dispatch->publishConfig(config_);
    auto cell_type = CellFactory::kPublisher;
    matrix_ = std::make_shared<Matrix>(rows, columns, state_dispatch, cell_type); // todo: Matrix factory
    Rules::populate(matrix_);
    matrix_->print();
}

void GameOfLife::on_publish() {
    Rules::apply(matrix_);
    matrix_->print();
}