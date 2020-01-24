#pragma once
#include <iostream>
#include "game_of_life.h"
class rules {
public:
    static void populate(std::shared_ptr<matrix> matrix);
    static void apply(std::shared_ptr<matrix> matrix);
};
