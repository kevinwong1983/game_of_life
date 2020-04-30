#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <vector>
#include "publishable.h"
#include "engine.h"

using namespace std;

int global_count = 0;
class EngineSubscriber : public Publishable {
public:
    void on_publish() override {
        std::cout << "ASDASDASDASD" << std::endl;
        global_count ++;
    }
    int get_count() {
        return global_count;
    }
};

class EngineWrapper : public Engine {
public:
    EngineWrapper(boost::asio::io_context &ioc, boost::asio::chrono::seconds seconds):Engine(ioc, seconds) {
    }

    void tick_wrapper() {
        tick();
    }
};

TEST(Engine, when_tick_subscriber_handler_called) {
    global_count = 0;
    boost::asio::io_context ioc;
    auto p = std::make_shared<EngineSubscriber>();
    EXPECT_EQ(p->get_count(), 0);

    EngineWrapper engine{ioc, boost::asio::chrono::seconds(1)};
    engine.subscribe(p);
    EXPECT_EQ(p->get_count(), 0);

    engine.tick_wrapper();
    EXPECT_EQ(p->get_count(), 1);
    engine.tick_wrapper();
    EXPECT_EQ(p->get_count(), 2);
}


TEST(Engine, on_tick_subscriber_handler_not_called_when_subscriber_out_of_scope) {
    global_count = 0;
    boost::asio::io_context ioc;
    EngineWrapper engine{ioc, boost::asio::chrono::seconds(1)};
    {
        auto p = std::make_shared<EngineSubscriber>();
        engine.subscribe(p);
        engine.tick_wrapper();
        EXPECT_EQ(global_count, 1);
    }
    engine.tick_wrapper();
    EXPECT_EQ(global_count, 1);
}

class Subscriber2 : public Publishable {
public:
    Subscriber2(boost::asio::io_context &ioc, int stop_count):ioc_(ioc), count_(0), stop_count_(stop_count) {
    }
    void on_publish() override {
        std::cout << "ASDASDASDASD" << std::endl;
        count_ ++;
        if (count_ >= stop_count_){
            ioc_.stop();
        }
    }
    int get_count () {
        return count_;
    }
private:
    boost::asio::io_context &ioc_;
    int count_ = 0;
    int stop_count_ = 0;
};

TEST(Engine, engine_runs) {
    boost::asio::io_context ioc;
    auto stop = 3;
    auto p = std::make_shared<Subscriber2>(ioc, stop);

    Engine engine{ioc, boost::asio::chrono::seconds(1)};
    engine.subscribe(p);
    EXPECT_EQ(p->get_count(), 0);

    ioc.run();
    EXPECT_EQ(p->get_count(), stop);
}
