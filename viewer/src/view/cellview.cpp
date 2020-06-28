#include <iostream>

#include "cellview.h"
#include "ui_cellview.h"

CellView::CellView(QWidget *parent, int r, int c) :
        QFrame(parent),
        ui(new Ui::CellView),
        r_(r), c_(c) {
    ui->setupUi(this);
    setFrameShape(QFrame::Box);
    setLineWidth(1);
}

CellView::~CellView() {
    delete ui;
}

void CellView::mouseReleaseEvent(QMouseEvent *event) {
    (void) event;
    std::cout << __func__ << " r: " << r_ << " c:" << c_ << std::endl;
}

void CellView::OnStateChange(bool state) {
    UpdateBackground(state);
}

void CellView::UpdateBackground(bool state) {
    QPalette pal = this->palette();
    if (state) {
        pal.setColor(QPalette::Background, Qt::black);
    } else {
        pal.setColor(QPalette::Background, Qt::white);
    }
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void CellView::on_publish(int x, int y, bool s) {
    (void) x;
    (void) y;
    OnStateChange(s);
}
