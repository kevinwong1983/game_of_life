#include <iostream>
#include "game_engine.h"

game_engine::game_engine(boost::asio::io_context& ioc, boost::asio::chrono::seconds sec):
        timer_(ioc, sec), itterations_(0), timer_setting_in_seconds_(sec) {
    timer_.async_wait(boost::bind(&game_engine::timer_handle, this));
}

game_engine::~game_engine() {
    timer_.cancel();
    event_publisher_.disconnect_all_slots();
}

void game_engine::timer_handle() {
    publish();
    timer_.expires_at(timer_.expires_at() + timer_setting_in_seconds_);
    timer_.async_wait(boost::bind(&game_engine::timer_handle, this));
}

void game_engine::publish() {
    event_publisher_();
}

void game_engine::subscribe(std::shared_ptr<Publishable> subscriber) {
    auto slot = boost::signals2::signal<void()>::slot_type(&Publishable::PublishHandler, subscriber.get());
    slot.track_foreign(subscriber);
    event_publisher_.connect(slot);
}