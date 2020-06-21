#include <iostream>
#include <random>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock/mock_state_dispatcher.h"
#include "matrix.h"
#include "rules.h"
#include "cell_factory.h"

using namespace std;

class RuleTest : public testing::Test {
public:
    void SetUp(){
        state_dispatcher_mock = std::make_shared<StateDispatcherMock>();
    }
    void TearDown(){

    }

    std::string kPublisher_ = CellFactory::kPublisher;
    std::string kSubscriber_ = CellFactory::kSubscriber;
    std::shared_ptr<StateDispatcherMock> state_dispatcher_mock;
};

TEST_F(RuleTest, random) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);
    auto liveGenerator = std::bind(distribution, generator);

    int total = 0;
    for (int i  = 0; i < 1000; i++) {
        total += liveGenerator();
    }
    EXPECT_LT(total, 550);
    EXPECT_GT(total, 450);
}

TEST_F(RuleTest, Any_live_cell_with_fewer_than_two_live_neighbours_dies_nominal) {
    int row = 3;
    int column = 3;
    auto m = std::make_shared<Matrix>(row, column, state_dispatcher_mock, kPublisher_);

    //    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    m->set(0,0,false); m->set(0,1,true);  m->set(0,2,false);
    m->set(1,0,false); m->set(1,1,true);  m->set(1,2,false);
    m->set(2,0,false); m->set(2,1,false); m->set(2,2,false);
    m->print();

    Rules::apply(m);

    EXPECT_EQ(false, m->get(0,0));   EXPECT_EQ(false, m->get(0,1));  EXPECT_EQ(false, m->get(0,2));
    EXPECT_EQ(false, m->get(1,0));   EXPECT_EQ(false, m->get(1,1));  EXPECT_EQ(false, m->get(1,2));
    EXPECT_EQ(false, m->get(2,0));   EXPECT_EQ(false, m->get(2,1));  EXPECT_EQ(false, m->get(2,2));
    m->print();
}

TEST_F(RuleTest, blinker_test_case) {
    int row = 5;
    int column = 5;
    int test_periods = 10;
    auto m = std::make_shared<Matrix>(row, column, state_dispatcher_mock, kPublisher_);

    m->set(0,0,false); m->set(0,1,false); m->set(0,2,false); m->set(0,3,false); m->set(0,4,false);
    m->set(1,0,false); m->set(1,1,false); m->set(1,2,true);  m->set(1,3,false); m->set(1,4,false);
    m->set(2,0,false); m->set(2,1,false); m->set(2,2,true);  m->set(2,3,false); m->set(2,4,false);
    m->set(3,0,false); m->set(3,1,false); m->set(3,2,true);  m->set(3,3,false); m->set(3,4,false);
    m->set(4,0,false); m->set(4,1,false); m->set(4,2,false); m->set(4,3,false); m->set(4,4,false);
    m->print();

    function<void()> blink = [&](){
        Rules::apply(m);
        EXPECT_EQ(false, m->get(0,0));   EXPECT_EQ(false, m->get(0,1));  EXPECT_EQ(false, m->get(0,2)); EXPECT_EQ(false, m->get(0,3)); EXPECT_EQ(false, m->get(0,4));
        EXPECT_EQ(false, m->get(1,0));   EXPECT_EQ(false, m->get(1,1));  EXPECT_EQ(false, m->get(1,2)); EXPECT_EQ(false, m->get(1,3)); EXPECT_EQ(false, m->get(1,4));
        EXPECT_EQ(false, m->get(2,0));   EXPECT_EQ(true,  m->get(2,1));  EXPECT_EQ(true,  m->get(2,2)); EXPECT_EQ(true,  m->get(2,3)); EXPECT_EQ(false, m->get(2,4));
        EXPECT_EQ(false, m->get(3,0));   EXPECT_EQ(false, m->get(3,1));  EXPECT_EQ(false, m->get(3,2)); EXPECT_EQ(false, m->get(3,3)); EXPECT_EQ(false, m->get(3,4));
        EXPECT_EQ(false, m->get(4,0));   EXPECT_EQ(false, m->get(4,1));  EXPECT_EQ(false, m->get(4,2)); EXPECT_EQ(false, m->get(4,3)); EXPECT_EQ(false, m->get(4,4));
        m->print();
        Rules::apply(m);
        EXPECT_EQ(false, m->get(0,0));   EXPECT_EQ(false, m->get(0,1));  EXPECT_EQ(false, m->get(0,2)); EXPECT_EQ(false, m->get(0,3)); EXPECT_EQ(false, m->get(0,4));
        EXPECT_EQ(false, m->get(1,0));   EXPECT_EQ(false, m->get(1,1));  EXPECT_EQ(true,  m->get(1,2)); EXPECT_EQ(false, m->get(1,3)); EXPECT_EQ(false, m->get(1,4));
        EXPECT_EQ(false, m->get(2,0));   EXPECT_EQ(false, m->get(2,1));  EXPECT_EQ(true,  m->get(2,2)); EXPECT_EQ(false, m->get(2,3)); EXPECT_EQ(false, m->get(2,4));
        EXPECT_EQ(false, m->get(3,0));   EXPECT_EQ(false, m->get(3,1));  EXPECT_EQ(true,  m->get(3,2)); EXPECT_EQ(false, m->get(3,3)); EXPECT_EQ(false, m->get(3,4));
        EXPECT_EQ(false, m->get(4,0));   EXPECT_EQ(false, m->get(4,1));  EXPECT_EQ(false, m->get(4,2)); EXPECT_EQ(false, m->get(4,3)); EXPECT_EQ(false, m->get(4,4));
        m->print();
    };

    for (int i = 0; i <test_periods; i++) {
        blink();
    }
}

TEST_F(RuleTest, beacon_test_case) {
    int row = 6;
    int column = 6;
    int test_periods = 10;
    auto m = std::make_shared<Matrix>(row, column, state_dispatcher_mock, kPublisher_);

    m->set(0,0,false); m->set(0,1,false); m->set(0,2,false); m->set(0,3,false); m->set(0,4,false); m->set(0,5,false);
    m->set(1,0,false); m->set(1,1,true);  m->set(1,2,true);  m->set(1,3,false); m->set(1,4,false); m->set(1,5,false);
    m->set(2,0,false); m->set(2,1,true);  m->set(2,2,false); m->set(2,3,false); m->set(2,4,false); m->set(2,5,false);
    m->set(3,0,false); m->set(3,1,false); m->set(3,2,false); m->set(3,3,false); m->set(3,4,true);  m->set(3,5,false);
    m->set(4,0,false); m->set(4,1,false); m->set(4,2,false); m->set(4,3,true);  m->set(4,4,true);  m->set(4,5,false);
    m->set(5,0,false); m->set(5,1,false); m->set(5,2,false); m->set(5,3,false); m->set(5,4,false); m->set(5,5,false);
    m->print();

    function<void()> beacon = [&](){
        Rules::apply(m);
        EXPECT_EQ(false, m->get(0,0));   EXPECT_EQ(false, m->get(0,1));  EXPECT_EQ(false, m->get(0,2)); EXPECT_EQ(false, m->get(0,3)); EXPECT_EQ(false, m->get(0,4)); EXPECT_EQ(false, m->get(0,5));
        EXPECT_EQ(false, m->get(1,0));   EXPECT_EQ(true, m->get(1,1));   EXPECT_EQ(true, m->get(1,2));  EXPECT_EQ(false, m->get(1,3)); EXPECT_EQ(false, m->get(1,4)); EXPECT_EQ(false, m->get(1,5));
        EXPECT_EQ(false, m->get(2,0));   EXPECT_EQ(true, m->get(2,1));   EXPECT_EQ(true, m->get(2,2));  EXPECT_EQ(false, m->get(2,3)); EXPECT_EQ(false, m->get(2,4)); EXPECT_EQ(false, m->get(2,5));
        EXPECT_EQ(false, m->get(3,0));   EXPECT_EQ(false, m->get(3,1));  EXPECT_EQ(false, m->get(3,2)); EXPECT_EQ(true, m->get(3,3));  EXPECT_EQ(true, m->get(3,4));  EXPECT_EQ(false, m->get(3,5));
        EXPECT_EQ(false, m->get(4,0));   EXPECT_EQ(false, m->get(4,1));  EXPECT_EQ(false, m->get(4,2)); EXPECT_EQ(true, m->get(4,3));  EXPECT_EQ(true, m->get(4,4));  EXPECT_EQ(false, m->get(4,5));
        EXPECT_EQ(false, m->get(5,0));   EXPECT_EQ(false, m->get(5,1));  EXPECT_EQ(false, m->get(5,2)); EXPECT_EQ(false, m->get(5,3)); EXPECT_EQ(false, m->get(5,4)); EXPECT_EQ(false, m->get(5,5));
        m->print();
        Rules::apply(m);
        EXPECT_EQ(false, m->get(0,0));   EXPECT_EQ(false, m->get(0,1));  EXPECT_EQ(false, m->get(0,2)); EXPECT_EQ(false, m->get(0,3)); EXPECT_EQ(false, m->get(0,4)); EXPECT_EQ(false, m->get(0,5));
        EXPECT_EQ(false, m->get(1,0));   EXPECT_EQ(true, m->get(1,1));   EXPECT_EQ(true, m->get(1,2));  EXPECT_EQ(false, m->get(1,3)); EXPECT_EQ(false, m->get(1,4)); EXPECT_EQ(false, m->get(1,5));
        EXPECT_EQ(false, m->get(2,0));   EXPECT_EQ(true, m->get(2,1));   EXPECT_EQ(false, m->get(2,2)); EXPECT_EQ(false, m->get(2,3)); EXPECT_EQ(false, m->get(2,4)); EXPECT_EQ(false, m->get(2,5));
        EXPECT_EQ(false, m->get(3,0));   EXPECT_EQ(false, m->get(3,1));  EXPECT_EQ(false, m->get(3,2)); EXPECT_EQ(false, m->get(3,3)); EXPECT_EQ(true, m->get(3,4));  EXPECT_EQ(false, m->get(3,5));
        EXPECT_EQ(false, m->get(4,0));   EXPECT_EQ(false, m->get(4,1));  EXPECT_EQ(false, m->get(4,2)); EXPECT_EQ(true, m->get(4,3));  EXPECT_EQ(true, m->get(4,4));  EXPECT_EQ(false, m->get(4,5));
        EXPECT_EQ(false, m->get(5,0));   EXPECT_EQ(false, m->get(5,1));  EXPECT_EQ(false, m->get(5,2)); EXPECT_EQ(false, m->get(5,3)); EXPECT_EQ(false, m->get(5,4)); EXPECT_EQ(false, m->get(5,5));
        m->print();
    };

    for (int i = 0; i <test_periods; i++) {
        beacon();
    }
}


