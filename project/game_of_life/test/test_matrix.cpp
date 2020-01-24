#include <iostream>
#include <gtest/gtest.h>
#include "matrix.h"

using namespace std;

TEST(matrix_test, test) {
    EXPECT_EQ(1, 1);
}

TEST(matrix_test, print) {
    matrix m(5,10);
    m.print();
}

TEST(matrix_test, state_of_cells_after_init_should_be_false) {
    int row = 5;
    int column = 10;
    matrix m(row, column);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            EXPECT_EQ(false, m.get(j, i));
        }
    }

    std::vector<int> b = {1, 2, 3, 4, 5};
    std::unique_ptr<std::vector<int>> a = std::make_unique<std::vector<int>>(std::move(b));
    EXPECT_EQ(1, (*a)[0]);
    EXPECT_EQ(2, (*a)[1]);
    EXPECT_EQ(3, (*a)[2]);
    EXPECT_EQ(4, (*a)[3]);
    EXPECT_EQ(5, (*a)[4]);

    std::vector<std::vector<int>> d;
    d.emplace_back(std::vector<int>{1,2,3,4});
    d.emplace_back(std::vector<int>{3,4,5,6});
    EXPECT_EQ(1, d[0][0]);

    std::unique_ptr<std::vector<std::vector<int>>> e = std::make_unique<std::vector<std::vector<int>>>();
    e->emplace_back(std::vector<int>{1,2,3,4});
    e->emplace_back(std::vector<int>{3,4,5,6});
    EXPECT_EQ(1, (*e)[0][0]);
    EXPECT_EQ(2, (*e)[0][1]);
    EXPECT_EQ(3, (*e)[0][2]);
}

TEST(matrix_test, state_of_cell_can_be_set) {
    int row = 5;
    int column = 10;
    matrix m(row, column);
    //    0 0 0 0 0 -> rows = x
    //    0 0 0 0 0
    //    0 0 0 0 0
    //    0 0 0 0 0
    //    0 0 0 0 0
    //    0 0 0 0 0
    //    0 0 0 0 0
    //    0 0 0 0 0
    //    0 0 0 0 0
    m.print();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            EXPECT_EQ(false, m.get(j, i));
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            m.set(j, i, true);
        }
    }

    m.print();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            EXPECT_EQ(true, m.get(j, i));
        }
    }
}


TEST(matrix_test, mark_and_apply_matrix_for_state_change) {
    int row = 5;
    int column = 10;
    matrix m(row, column);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            m.mark(j, i, true);
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            EXPECT_EQ(false, m.get(j, i));
        }
    }

    m.applyMarked();

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            EXPECT_EQ(true, m.get(j, i));
        }
    }

}
