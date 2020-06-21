#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mock/mock_state_dispatcher.h"
#include "cell_factory.h"
#include "matrix.h"

using namespace std;

class MatrixTest : public testing::Test {
public:
    void SetUp() {
        state_dispatcher_mock = std::make_shared<StateDispatcherMock>();
    }

    void TearDown() {
    }

    std::string kPublisher_ = CellFactory::kPublisher;
    std::string kSubscriber_ = CellFactory::kSubscriber;
    std::shared_ptr<StateDispatcherMock> state_dispatcher_mock;
};

TEST_F(MatrixTest, test) {
    EXPECT_EQ(1, 1);
}

TEST_F(MatrixTest, print) {

    Matrix m(5, 10, state_dispatcher_mock, kPublisher_);
    m.print();
}

TEST_F(MatrixTest, state_of_cells_after_init_should_be_false) {
    int row = 5;
    int column = 10;
    Matrix m(row, column, state_dispatcher_mock, kPublisher_);

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
    d.emplace_back(std::vector<int>{1, 2, 3, 4});
    d.emplace_back(std::vector<int>{3, 4, 5, 6});
    EXPECT_EQ(1, d[0][0]);

    std::unique_ptr<std::vector<std::vector<int>>> e = std::make_unique<std::vector<std::vector<int>>>();
    e->emplace_back(std::vector<int>{1, 2, 3, 4});
    e->emplace_back(std::vector<int>{3, 4, 5, 6});
    EXPECT_EQ(1, (*e)[0][0]);
    EXPECT_EQ(2, (*e)[0][1]);
    EXPECT_EQ(3, (*e)[0][2]);
}

TEST_F(MatrixTest, state_of_cell_can_be_set) {
    int row = 5;
    int column = 10;
    Matrix m(row, column, state_dispatcher_mock, kPublisher_);
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


TEST_F(MatrixTest, mark_and_apply_matrix_for_state_change) {
    int row = 5;
    int column = 10;
    Matrix m(row, column, state_dispatcher_mock, kPublisher_);

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


TEST_F(MatrixTest, print2) {
    int row = 2;
    int column = 3;
    Matrix m(row, column, state_dispatcher_mock, kPublisher_);
    m.print();
}
