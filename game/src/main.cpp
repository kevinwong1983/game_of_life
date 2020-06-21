#include <iostream>
#include "game_of_life.h"
#include "engine.h"
#include "state_publisher_cell.h"
#include <boost/asio.hpp>

int main() {
    boost::asio::io_context ioc;
    const std::string id = "game_of_life_" + std::to_string(getppid());
    std::string ip = "127.0.0.1";
    int port = 1883;
    int row = 10;
    int column = 12;
    auto seconds = boost::asio::chrono::seconds(1);

    auto state_dispatch = std::make_shared<StateDispatcherCell>(ioc, id, ip, port);
    auto gol = std::make_shared<GameOfLife>(row, column, state_dispatch);
    Engine engine(ioc, seconds);
    engine.subscribeToTicks(gol);

    ioc.run();

    return 0;
}