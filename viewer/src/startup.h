#pragma once

#include <QObject>
#include "view/mainwindow.h"
#include "configuration.h"
#include "matrix.h"
#include "model/game.h"

class Startup : public QObject {
Q_OBJECT
public:
    explicit Startup(Game &game);

    ~Startup();

    void show() const;

    void SetConfig(std::shared_ptr<Configuration> configuration);

signals:

    void NotifyStartup(std::shared_ptr<Configuration> config);

public slots:

    void OnStartup(std::shared_ptr<Configuration> config);

private:
    void WireStartupConnections();

    void SetWindowSize(int rows, int columns);

    Game &game_;
    GameView &game_view_;
    Stats &stats_;
    std::unique_ptr<MainWindow> main_window_;
};
