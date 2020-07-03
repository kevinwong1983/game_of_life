//
// Created by user on 2020-04-23.
//
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/system_error.hpp>
#include <iostream>
#include <boost/signals2.hpp>
#include "engine.h"

Engine::Engine(boost::asio::io_context &ioc, boost::asio::chrono::seconds seconds) : seconds_(seconds) {
    t_ = std::make_shared<boost::asio::steady_timer>(ioc, seconds);
    t_->async_wait(boost::bind(&Engine::timer_handler, this, boost::asio::placeholders::error));
}

Engine::~Engine() {
    try {
        subscribers_.disconnect_all_slots();
    }
    catch(std::exception& e) {
        std::cout << "not able to disconnect subscriber slots with error code:" << e.what() << std::endl;
    }
}

void Engine::timer_handler(boost::system::error_code e) {
    if (!e) {
        tick();
        t_->expires_at(t_->expiry() + seconds_);
        t_->async_wait(boost::bind(&Engine::timer_handler, this, boost::asio::placeholders::error));
    } else {
        std::cout << "error: " << __func__ << " error_code: " << e << std::endl;
    }
}

void Engine::tick() {
    std::cout << "tick" << std::endl;
    subscribers_();
}

void Engine::subscribeToTicks(std::shared_ptr<Publishable> on_tick) {
    auto scope_tracked_connection = boost::signals2::signal<void()>::slot_type(&Publishable::on_publish,
                                                                               on_tick.get()).track_foreign(
            on_tick);
    subscribers_.connect(scope_tracked_connection);
}


