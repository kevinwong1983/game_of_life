#pragma once
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include "publishable.h"

class game_engine {
public:
    game_engine(boost::asio::io_context& ioc, boost::asio::chrono::seconds sec);
    ~game_engine();
    void subscribe(std::shared_ptr<Publishable> subscriber);
    void publish();

protected:
    void timer_handle();
    int itterations_;
    boost::asio::steady_timer timer_;
private:
    boost::asio::chrono::seconds timer_setting_in_seconds_;
    boost::signals2::signal<void()> event_publisher_;
};
