#pragma once

#include <iostream>
#include <vector>
#include "cell.h"
#include "publishable.h"
#include "matrix.h"
#include "state_dispatcher.h"

class GameOfLife : public Publishable {
public:
    GameOfLife(int rows, int columns, std::shared_ptr<StateDispatcher> state_dispatch);
    void on_publish() override;
private:
    std::shared_ptr<Matrix> matrix_;
};
