#include <random>

#include "rules.h"

void Rules::apply(std::shared_ptr<Matrix> m) {
    auto column = m->getColumnSize();
    auto row = m->getRowSize();

    for (auto x = 0; x < column; x++) {
        for (auto y = 0; y < row; y++) {
            auto neighbours = m->getNeighbours(x, y);
            auto alive = m->get(x, y);
            // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            if (alive && (neighbours < 2)) {    // NOLINT: suppress "bugprone-branch-clone" for understandablity
                m->mark(x, y, false);
            }
            // Any live cell with two or three live neighbours lives on to the next generation.
            else if (alive && ((neighbours == 2) || (neighbours == 3))) {     // NOLINT: suppress "bugprone-branch-clone" for understandablity
                m->mark(x, y, true);
            }
            // Any live cell with more than three live neighbours dies, as if by overpopulation.
            else if (alive && (neighbours > 3)) {
                m->mark(x, y, false);
            }
            // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            else if (!alive && (neighbours == 3)) {
                m->mark(x, y, true);
            }
        }
    }

    m->applyMarked();
}

void Rules::populate(std::shared_ptr<Matrix> m) {
    auto column = m->getColumnSize();
    auto row = m->getRowSize();
    std::cout << "r: " << row << " c: " << column << std::endl;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 1);
    auto live_generator = std::bind(distribution, generator);

    for (int x = 0; x < column; x++) {
        for (int y = 0; y < row; y++) {
            bool is_alive = live_generator() ? true : false;
            m->set(x, y, is_alive);
        }
    }
}