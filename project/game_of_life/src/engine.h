#pragma once

#include <boost/system/system_error.hpp>
#include <boost/signals2.hpp>
#include <boost/asio.hpp>
#include "publishable.h"

class Engine {
public:
    Engine(boost::asio::io_context &ioc, boost::asio::chrono::seconds seconds);
    ~Engine();
    void subscribe(std::shared_ptr<Publishable> Publishable);

protected:
    void tick();

private:
    void timer_handler(boost::system::error_code e);

    boost::asio::chrono::seconds seconds_;
    std::shared_ptr<boost::asio::steady_timer> t_;
    boost::signals2::signal<void()> subscribers;
};