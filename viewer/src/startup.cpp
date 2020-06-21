//
// Created by user on 17/06/2020.
//
#include <iostream>
#include <QEventLoop>
#include "startup.h"

Startup::Startup(Game &game) : game_(game), game_view_(*new GameView(nullptr, game)),
                               stats_(*new Stats(nullptr)),
                               main_window_(std::make_unique<MainWindow>(nullptr, game_view_, stats_)) {
    WireStartupConnections();
}

Startup::~Startup() {
}

void Startup::SetConfig(std::shared_ptr<Configuration> configuration) {
    if (configuration->rows_ && configuration->columns_) {
        emit NotifyStartup(configuration); // use emit to decouple the ioc and qt_exec threads
    }
}

void Startup::OnStartup(std::shared_ptr<Configuration> config) {
    if (config->rows_ && config->columns_) {
        std::cout << __func__ << " r:" << config->rows_.get() << " c:" << config->columns_.get() << std::endl;
        game_.CreateGameWorld(config);
        game_view_.DrawGameWorld(config->rows_.get(), config->columns_.get());
        SetWindowSize(config->rows_.get(), config->columns_.get());
    }
}

void Startup::show() const {
    main_window_->show();
}

void Startup::SetWindowSize(int rows, int columns) {
    main_window_->setMaximumSize(columns * 20 + columns + 50,
                                 rows * 20 + rows + 100);
    main_window_->setMinimumSize(columns * 20 + columns + 50,
                                 rows * 20 + rows + 100);
}

void Startup::WireStartupConnections() {
    qRegisterMetaType<std::shared_ptr<Configuration>>("std::shared_ptr<Configuration>");
    connect(this, &Startup::NotifyStartup, this, &Startup::OnStartup);  // todo: can I use a smart pointer?
}