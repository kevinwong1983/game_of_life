#include <iostream>
#include <QtCore/Qt>
#include <QFrame>
#include <QtGui/QColor>
#include <QPainter>

#include "gameview.h"
#include "ui_gameview.h"
#include "cellview.h"

GameView::GameView(QWidget *parent, Game &game) :
        QWidget(parent),
        ui(new Ui::GameView),
        game_(game) {
    ui->setupUi(this);
}

GameView::~GameView() {
    delete ui;
}

void GameView::DrawGameWorld(int rows, int columns) {
    std::vector<std::vector<std::shared_ptr<CellView>>> cell_matrix_view;
    std::generate_n(std::back_inserter(cell_matrix_view), columns, [this, x = 0, &rows]()mutable {
        std::vector<std::shared_ptr<CellView>> inner;
        std::generate_n(std::back_inserter(inner), rows, [this, y = 0, &x]()mutable {
            auto cell = std::make_shared<CellView>(this, x, y);
            cell->UpdateBackground(game_.GetPixelState(x, y));
            cell->move(x * uc_ + x, y * uc_ + y);
            cell->show();
            game_.WireGamePixel(x, y, cell);
            y++;
            return cell;
        });
        x++;
        return inner;
    });
    cell_matrix_view_ = std::make_unique<std::vector<std::vector<std::shared_ptr<CellView>>>>(
            std::move(cell_matrix_view));
}
