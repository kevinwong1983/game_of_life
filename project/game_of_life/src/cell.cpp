#include "cell.h"

Cell::Cell(bool state, int x, int y):state_(state), marked_(false), x_(x), y_(y) {
}

void Cell::mark_state(bool mark) {
    marked_ = mark;
}

void Cell::update() {
    if (get_state() != marked_)
    {
        set_state(marked_);
    }
}

bool Cell::get_state() const {
    return state_;
}

int Cell::get_x() const {
    return x_;
}

int Cell::get_y() const {
    return y_;
}

void Cell::set_state(bool state) {
    state_ = state;
    state_publisher_(x_, y_, state_);
}

void Cell::subscribe(std::shared_ptr<PublishableWithContext> subscriber) {
    auto scoped_connection = boost::signals2::signal<void(int x, int y, bool s)>::slot_type(&PublishableWithContext::on_publish, subscriber.get(), boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3)
            .track_foreign(subscriber);
    state_publisher_.connect(scoped_connection);
}