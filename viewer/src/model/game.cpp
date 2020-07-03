//
// Created by user on 13/06/2020.
//
#include "game.h"
#include "matrix.h"

Game::Game(std::shared_ptr<StatePublisher> dispatcher) :
        dispatcher_(std::move(dispatcher)), row_(0),
        column_(0), matrix_(std::make_unique<Matrix>(row_, column_, dispatcher_,
                                                     kCellType_)) {
    // todo: question - within a class binding to member function in constructor, that is not static
}

void Game::CreateGame(std::shared_ptr<Configuration> configuration) {
    if (configuration->rows_ && configuration->columns_) {
        matrix_ = std::make_unique<Matrix>(configuration->rows_.get(), configuration->columns_.get(), dispatcher_,
                                           kCellType_); // todo: Matrix factory
    }
}

void Game::WireGamePixel(int x, int y, std::shared_ptr<CellView> cell) {
    matrix_->subscribeToCell(x, y, cell);
}

bool Game::GetPixelState(int x, int y) const {
    return matrix_->get(x, y);
}