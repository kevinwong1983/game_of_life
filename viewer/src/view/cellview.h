#ifndef CELLVIEW_H
#define CELLVIEW_H

#include <QFrame>
#include "state_publisher_abstract.h"

namespace Ui {
class CellView;
}

class CellView : public QFrame, public StatePublisherAbstract
{
    Q_OBJECT

signals:
    void NotifyMouseEvent();

public slots:
    void OnStateChange(bool state);

public:
    explicit CellView(QWidget *parent, int r, int c);
    ~CellView();
    void UpdateBackground(bool state);

private:
    void on_publish(int x, int y, bool s) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    Ui::CellView *ui;
    int r_;
    int c_;
};

#endif // CELLVIEW_H
