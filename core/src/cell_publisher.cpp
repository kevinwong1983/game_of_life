#include <iostream>
#include "cell_publisher.h"

CellPublisher::CellPublisher(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set) : state_(state),
                                                                                                       marked_(false),
                                                                                                       x_(x), y_(y) {
    subscribeToStateSet(on_state_set);
}

CellPublisher::~CellPublisher() {
}

CellPublisher::CellPublisher(const CellPublisher &that) : x_(that.x_), y_(that.y_), marked_(that.marked_),
                                                          state_(that.state_) {
    notify_state_change_.connect(that.notify_state_change_);
}

CellPublisher &CellPublisher::operator=(const CellPublisher &that) {
    x_ = that.x_;
    y_ = that.y_;
    marked_ = that.marked_;
    state_ = that.state_;
    notify_state_change_.connect(that.notify_state_change_);
    return *this;
}

void CellPublisher::mark_state(bool mark) {
    marked_ = mark;
}

void CellPublisher::update() {
    if (get_state() != marked_) {
        set_state(marked_);
    }
}

bool CellPublisher::get_state() const {
    return state_;
}

int CellPublisher::get_x() const {
    return x_;
}

int CellPublisher::get_y() const {
    return y_;
}

void CellPublisher::set_state(bool state) {
    state_ = state;
    notify_state_change_(x_, y_, state_);
}

void CellPublisher::subscribeToStateSet(std::shared_ptr<StatePublisher> on_state_set) {
    auto scoped_connection = boost::signals2::signal<void(int x, int y, bool s)>::slot_type(
            &StatePublisher::on_publish, on_state_set.get(), boost::placeholders::_1, boost::placeholders::_2,
            boost::placeholders::_3)
            .track_foreign(on_state_set);
    notify_state_change_.connect(scoped_connection);
}

void CellPublisher::subscribe(std::shared_ptr<StatePublisherAbstract> on_state_set) {
    std::cout << "not implemented" << std::endl;
}
