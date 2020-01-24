#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>

#include "game_engine.h"
#include "publishable.h"

using namespace std;

class game_engine_wrapper: public game_engine {
public:
    game_engine_wrapper(boost::asio::io_context& ioc, boost::asio::chrono::seconds sec):game_engine(ioc,sec){};
    int get_itteration(){
        return itterations_;
    }
};

class game_engine_test : public ::testing::Test {
public:
    void SetUp() override {
        glw_ = std::make_shared<game_engine_wrapper>(ioc_, boost::asio::chrono::seconds(60));
    }

    void TearDown() override {
    }

    boost::asio::io_context ioc_;   // ioc should never go out of scope!
    std::shared_ptr<game_engine_wrapper> glw_;
};

TEST_F(game_engine_test, init) {
    EXPECT_EQ(0, glw_->get_itteration());
}

TEST_F(game_engine_test, loop_publish_with_no_subscribers) {
    glw_->publish();
    EXPECT_EQ(1, glw_->get_itteration());
}

// example of using interfaces
class Dier {
public:
    virtual void lopen() = 0;
};

class Aap : public Dier {
public:
    void lopen() override {
        std::cout << "apen loopje" << std::endl;
    }
};

class Geit : public Dier {
public:
    void lopen() override {
        std::cout << "geiten loopje" <<std::endl;
    }
};

void func1 (std::shared_ptr<Dier> d) {
    d->lopen();
}

class subscriber_class : public Publishable {
public:
    subscriber_class(int& count):count_(count){
        cout << "constructor dummy class" << endl;
    }
    ~subscriber_class(){
        cout << "destructor dummy class" << endl;
    }
    void PublishHandler() override {
        count_++;
    }

    int& count_;
};

TEST_F(game_engine_test, abstract_interface_tests) {
    auto a = std::make_shared<Aap>();
    auto g = std::make_shared<Geit>();
    a->lopen();
    func1(a);
    func1(g);
}

TEST_F(game_engine_test, loop_subscribed_wil_be_called) {
    int count = 0;
    auto sc = make_shared<subscriber_class>(count);
    glw_->subscribe(sc);
    glw_->publish();

    EXPECT_EQ(1, sc->count_);
    EXPECT_EQ(1, count);
}

TEST_F(game_engine_test, loop_subscribed_wil_not_be_called_is_subscriber_is_deleted) {
    int count = 0;

    {
        auto sc = make_shared<subscriber_class>(count);
        glw_->subscribe(sc);
    }   // subscriber_class is out of scope

    glw_->publish();
    EXPECT_EQ(0, count);    // count not increase
}

TEST_F(game_engine_test, loop_subscribed_wil_not_be_called_is_subscriber_is_deleted2) {
    int count = 0;

    auto sc = make_shared<subscriber_class>(count);
    glw_->subscribe(sc);

    EXPECT_EQ(1, sc.use_count());
    glw_->publish();
    EXPECT_EQ(1, count);    // count increase

    sc.reset();

    EXPECT_EQ(0, sc.use_count());
    glw_->publish();
    EXPECT_EQ(1, count);    // count not increase
}