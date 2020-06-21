#include <iostream>
#include <boost/asio.hpp>
#include <QApplication>
#include <QPushButton>
#include "state_publisher_cell.h"
#include "game.h"
#include "startup.h"
#include "boost/program_options.hpp"

void wait(boost::asio::io_context &ioc) {   // todo: probably there is a better way?
    ioc.post([&ioc]() {
        wait(ioc);
    });
}

struct ProgramOptions{
    std::string ip;
    int port;
};

void SetProgramOptions(int argc, char *argv[], ProgramOptions& po) {
    boost::program_options::options_description desc("program options");
    desc.add_options()
            ("help,h", "print info")
            ("mqtt.ip,i", boost::program_options::value<std::string>(&po.ip)->default_value("127.0.0.1"), "mqtt ip-adress.")
            ("mqtt.port,p", boost::program_options::value<int>(&po.port)->default_value(1883), "mqtt port number.");

    boost::program_options::variables_map vm;
    auto parsed = boost::program_options::parse_command_line(argc, argv, desc);
    boost::program_options::store(parsed, vm);
    boost::program_options::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(1);
    }
}

int main(int argc, char **argv) {
    ProgramOptions po;
    boost::asio::io_context ioc;
    const std::string id = "viewer_" + std::to_string(getppid());
    SetProgramOptions(argc, argv, po);

    auto state_dispatch = std::make_shared<StateDispatcherCell>(ioc, id, po.ip, po.port);

    QApplication app(argc, argv);
    Game game(state_dispatch);
    Startup startup(game);
    state_dispatch->subscribeConfig(std::bind(&Startup::SetConfig,
                                              &startup,
                                              std::placeholders::_1));
    auto t = std::thread([&ioc]() {
        wait(ioc);
        ioc.run();
    });
    startup.show();
    auto rv = app.exec();
    t.join();

    return rv;
}