#pragma once

#include <QMainWindow>

#include "gameview.h"
#include "stats.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, GameView &game, Stats &stats);

    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
};
