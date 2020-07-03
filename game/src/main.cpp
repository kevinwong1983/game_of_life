#include <iostream>
#include "game_of_life.h"
#include "engine.h"
#include "state_publisher_cell.h"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

struct ProgramOptions {
    int row = 0;
    int column = 0;
    std::string ip = "";
    int refresh_time = 0;
    int port = 0;
};

void SetProgramOptions(int argc, char *argv[], ProgramOptions& po) {  // NOLINT: keep argument list same as main
    const int k_default_game_row = 10;
    const int k_default_game_column = 11;
    const int k_default_mqtt_port = 1883;
    const int k_default_game_speed = 1;
    boost::program_options::options_description desc("program options");
    desc.add_options()
            ("help,h", "print info")
            ("game.row,r", boost::program_options::value<int>(&po.row)->default_value(k_default_game_row), "number of rows.")
            ("game.column,c", boost::program_options::value<int>(&po.column)->default_value(k_default_game_column), "number of columns.")
            ("mqtt.ip,i", boost::program_options::value<std::string>(&po.ip)->default_value("127.0.0.1"), "mqtt ip-adress.")
            ("mqtt.port,p", boost::program_options::value<int>(&po.port)->default_value(k_default_mqtt_port), "mqtt port number.")
            ("game.speed,s", boost::program_options::value<int>(&po.refresh_time)->default_value(k_default_game_speed), "speed of game in seconds.");

    boost::program_options::variables_map vm;
    auto parsed = boost::program_options::parse_command_line(argc, argv, desc);
    boost::program_options::store(parsed, vm);
    boost::program_options::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    ProgramOptions po;
    boost::asio::io_context ioc;
    const std::string id = "game_of_life_" + std::to_string(getppid());
    SetProgramOptions(argc, argv, po);

    try{
        auto state_dispatch = std::make_shared<StateDispatcherCell>(ioc, id, po.ip, po.port);
        auto gol = std::make_shared<GameOfLife>(po.row, po.column, state_dispatch);
        Engine engine(ioc, boost::asio::chrono::seconds(po.refresh_time));
        engine.subscribeToTicks(gol);
        ioc.run();
    }
    catch(std::exception& e) {
        std::cout<< "expection occured in game: " << e.what() << std::endl;
    }

    return 0;
}