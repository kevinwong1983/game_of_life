#pragma once

#include <QWidget>
#include "cellview.h"
#include "matrix.h"
#include "game.h"

namespace Ui {
class GameView;
}

class GameView : public QWidget
{
    Q_OBJECT

public:
    explicit GameView(QWidget *parent, Game& game);
    ~GameView();
    void DrawGameWorld(int row, int columns);

private:
    Ui::GameView *ui;
    Game& game_;
    const int uc_ = 20;
    std::unique_ptr<std::vector<std::vector<std::shared_ptr<CellView>>>> cell_matrix_view_;
};
