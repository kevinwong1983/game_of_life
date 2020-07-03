#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <vector>
#include "game_of_life.h"
#include "mock_state_dispatcher.h"

using namespace std;

TEST(dummy, test) {
    EXPECT_EQ(1, 1);
}

TEST(game_of_life, construct) {
    int row = 5;
    int column = 10;
    boost::asio::io_context ioc;
    std::string id = "id";
    std::string ip = "123";

    auto mock = std::make_shared<StateDispatcherMock>();
    auto gol = GameOfLife(row, column, mock);
}