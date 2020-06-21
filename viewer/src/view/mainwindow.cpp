#include <iostream>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, GameView &game, Stats &stats) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    game.setParent(this);
    stats.setParent(this);
    ui->game_tab->addWidget(&game);
    ui->stats_tab->addWidget(&stats);
}

MainWindow::~MainWindow() {
    delete ui;
}

