#include <boost/lexical_cast.hpp>
#include <iostream>
#include "cell_subscriber.h"

CellSubscriber::CellSubscriber(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set) :
        x_(x), y_(y),
        marked_(false),
        state_(state),
        sc_(on_state_set->subscribe(x_, y_, std::bind(
        &CellSubscriber::subscribeHandler, this,
        std::placeholders::_1,
        std::placeholders::_2))) {
}

void CellSubscriber::subscribeHandler(const Topic &t, const Message &m) {
    std::cout << "t:" << t << " m:" << m << std::endl;
    try {
        auto state = boost::lexical_cast<bool>(m);
        set_state(state);
    }
    catch (const boost::bad_lexical_cast &e) {
        std::cout << e.what() << " message:" << m << std::endl;
    }
}

void CellSubscriber::mark_state(bool mark) {
    marked_ = mark;
}

void CellSubscriber::update() {
    if (get_state() != marked_) {
        set_state(marked_);
    }
}

bool CellSubscriber::get_state() const {
    return state_;
}

int CellSubscriber::get_x() const {
    return x_;
}

int CellSubscriber::get_y() const {
    return y_;
}

void CellSubscriber::set_state(bool state) {
    state_ = state;
    notify_state_change_(x_, y_, state_);
    std::cout << "x:" << x_ << " y:" << y_ << " state:" << state_ << std::endl;
}

void CellSubscriber::subscribe(std::shared_ptr<StatePublisherAbstract> on_state_set) {
    auto scoped_connection = boost::signals2::signal<void(int x, int y, bool s)>::slot_type(
            &StatePublisher::on_publish, on_state_set.get(), boost::placeholders::_1, boost::placeholders::_2,
            boost::placeholders::_3)
            .track_foreign(on_state_set);
    notify_state_change_.connect(scoped_connection);
}