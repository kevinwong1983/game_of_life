#pragma once

#include <QObject>

#include "configuration.h"
#include "matrix.h"
#include "view/mainwindow.h"
#include "model/game.h"

class Startup : public QObject {
Q_OBJECT
public:
    explicit Startup(Game &game);

    ~Startup() = default;

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
    const int k_cell_pixel_size_ = 20;
    const int k_padding_row_spacing_ = 100;
    const int k_padding_column_spacing_ = 50;
};
