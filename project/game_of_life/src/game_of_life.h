#pragma once

#include <iostream>
#include <vector>
#include "cell.h"
#include "matrix.h"
#include "game_engine.h"

class game_of_life : public Publishable {
public:
    game_of_life(int rows, int columns);
    void PublishHandler() override;
private:
    std::shared_ptr<matrix> matrix_;
};
