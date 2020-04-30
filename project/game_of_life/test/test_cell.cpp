#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <vector>

#include "cell.h"
#include "mock/mock_state_dispatcher.h"

using namespace std;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::_;

class CellTest : public testing::Test {
public:
    void SetUp() {
        state_dispatcher_mock = std::make_shared<StateDispatcherMock>();
    }
    void TearDown() {

    }
    std::shared_ptr<StateDispatcherMock> state_dispatcher_mock;
};

TEST_F(CellTest, constructor) {
    Cell cell1(true, 1, 1);
    EXPECT_EQ(cell1.get_state(), true);

    Cell cell2(false, 1, 1);
    EXPECT_EQ(cell2.get_state(), false);
}

TEST_F(CellTest, mark_and_update) {
    Cell c(false, 1, 1);
    c.mark_state(true);
    EXPECT_EQ(c.get_state(), false);
    c.update();
    EXPECT_EQ(c.get_state(), true);

    Cell d(true, 1, 1);
    d.mark_state(false);
    EXPECT_EQ(d.get_state(), true);
    d.update();
    EXPECT_EQ(d.get_state(), false);
}

int global_cell_count = 0;
class CellSubscriber : public PublishableWithContext {
    void on_publish(int x, int y, bool s) override {
        std::cout << "context x:" << std::to_string(x) << " y:" << std::to_string(x) << " s:" << std::to_string(s) << std::endl;
        global_cell_count ++;
    }
};

TEST_F(CellTest, state_chage_will_be_published_when_subscribed) {
    Cell c(false, 1, 1);
    c.subscribe(state_dispatcher_mock);

    bool expected_state;
    EXPECT_CALL(*state_dispatcher_mock, on_publish(_, _, _))
    .Times(2)
    .WillRepeatedly(Invoke([&expected_state](int x, int y, bool state){
        EXPECT_EQ(x, 1);
        EXPECT_EQ(y, 1);
        EXPECT_EQ(state, expected_state);
    }));

    expected_state = true;
    c.mark_state(expected_state);
    c.update();
    EXPECT_EQ(c.get_state(), true);

    expected_state = false;
    c.mark_state(expected_state);
    c.update();
    EXPECT_EQ(c.get_state(), false);
}

TEST_F(CellTest, set_state_will_be_published_when_subscribed) {
    Cell c(false, 1, 1);
    c.subscribe(state_dispatcher_mock);

    bool expected_state;
    EXPECT_CALL(*state_dispatcher_mock, on_publish(_, _, _))
            .Times(1)
            .WillRepeatedly(Invoke([&expected_state](int x, int y, bool state){
                EXPECT_EQ(x, 1);
                EXPECT_EQ(y, 1);
                EXPECT_EQ(state, expected_state);
            }));

    expected_state = true;
    c.set_state(expected_state);
    EXPECT_EQ(c.get_state(), true);
}

TEST_F(CellTest, state_chage_will_not_be_published_when_subscriber_is_out_of_scope) {
    global_cell_count = 0;

    Cell c(false, 1, 1);
    {
        auto s = std::make_shared<CellSubscriber>();
        c.subscribe(s);

        c.mark_state(true);
        c.update();
        EXPECT_EQ(c.get_state(), true);
        EXPECT_EQ(global_cell_count, 1);
    }
    c.mark_state(false);
    c.update();
    EXPECT_EQ(c.get_state(), false);
    EXPECT_EQ(global_cell_count, 1);
}