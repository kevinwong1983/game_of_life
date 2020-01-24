#include <iostream>
#include "game_of_life.h"

int main() {
    boost::asio::io_context ioc;
    auto refresh_rate = boost::asio::chrono::seconds(1);
    auto rows = 10;
    auto columns = 10;
    game_engine engine(ioc, refresh_rate);
    auto temp_game_of_life = std::make_shared<game_of_life>(rows, columns);
    engine.subscribe(temp_game_of_life);

    ioc.run();

    return 0;
}