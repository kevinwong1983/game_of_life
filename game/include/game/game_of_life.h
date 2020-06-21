#pragma once

#include <iostream>
#include <vector>
#include "publishable.h"
#include "matrix.h"
#include "state_publisher.h"
#include "configuration.h"

class GameOfLife : public Publishable {
public:
    GameOfLife(int rows, int columns, std::shared_ptr<StatePublisher> state_dispatch);
    void on_publish() override;
private:
    std::shared_ptr<Matrix> matrix_;
    std::shared_ptr<Configuration> config_;
};
