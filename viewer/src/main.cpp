#include <iostream>
#include <boost/asio.hpp>
#include <QApplication>
#include <QPushButton>
#include "state_publisher_cell.h"
#include "game.h"
#include "startup.h"

void wait(boost::asio::io_context &ioc) {   // todo: probably there is a better way?
    ioc.post([&ioc]() {
        wait(ioc);
    });
}

int main(int argc, char **argv) {
    boost::asio::io_context ioc;
    const std::string id = "viewer_" + std::to_string(getppid());
    std::string ip = "127.0.0.1";
    int port = 1883;

    auto state_dispatch = std::make_shared<StateDispatcherCell>(ioc, id, ip, port);

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