#include <iostream>
#include "game_of_life.h"
#include "engine.h"
#include "state_publisher_cell.h"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

struct ProgramOptions{
    int row;
    int column;
    std::string ip;
    int refresh_time;
    int port;
};

void SetProgramOptions(int argc, char *argv[], ProgramOptions& po) {
    boost::program_options::options_description desc("program options");
    desc.add_options()
            ("help,h", "print info")
            ("game.row,r", boost::program_options::value<int>(&po.row)->default_value(10), "number of rows.")
            ("game.column,c", boost::program_options::value<int>(&po.column)->default_value(11), "number of columns.")
            ("mqtt.ip,i", boost::program_options::value<std::string>(&po.ip)->default_value("127.0.0.1"), "mqtt ip-adress.")
            ("mqtt.port,p", boost::program_options::value<int>(&po.port)->default_value(1883), "mqtt port number.")
            ("game.speed,s", boost::program_options::value<int>(&po.refresh_time)->default_value(1), "speed of game in seconds.");

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
    auto state_dispatch = std::make_shared<StateDispatcherCell>(ioc, id, po.ip, po.port);
    auto gol = std::make_shared<GameOfLife>(po.row, po.column, state_dispatch);
    Engine engine(ioc, boost::asio::chrono::seconds(po.refresh_time));
    engine.subscribeToTicks(gol);
    ioc.run();

    return 0;
}