#pragma once

#include <memory>
#include <QObject>
#include "state_publisher_cell.h"
#include "configuration.h"
#include "matrix.h"
#include "cellview.h"
#include "cell_factory.h"

class Game : public QObject {
Q_OBJECT
public:
    Game(std::shared_ptr<StatePublisher> dispatcher);

    void CreateGameWorld(std::shared_ptr<Configuration> configuration); // todo: why use a shared pointer???

    void WireGamePixel(int x, int y, std::shared_ptr<CellView> cell);

    bool GetPixel(int x, int y) const;

signals:

private:
    std::shared_ptr<StatePublisher> dispatcher_;
    int row_;
    int column_;
    std::string kCellType_ = CellFactory::kSubscriber;

protected:
    std::unique_ptr<Matrix> matrix_;
};