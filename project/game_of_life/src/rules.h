#pragma once

#include <iostream>
#include "matrix.h"

class Rules {
public:
    static void populate(std::shared_ptr<Matrix> matrix);
    static void apply(std::shared_ptr<Matrix> matrix);
};
