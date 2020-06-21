#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <vector>
#include "mock_state_dispatcher.h"
#include "game.h"
#include <tao/json.hpp>
#include "configuration.h"
#include "mock_cell.h"

using namespace std;

TEST(Game, test) {
    EXPECT_EQ(1, 1);
}


class GameTest : public Game {
public:
    GameTest(std::shared_ptr<StatePublisher> dispatcher) : Game(dispatcher) {}

    std::unique_ptr<Matrix> GetMatrix() {
        return std::move(matrix_);
    };
};

TEST(Game, CreateGameWorld__Initialized__Pixel_are_all_set_to_false) {
    auto cell_mock_factory = std::make_unique<CellMockFactory>();
    std::vector<std::unique_ptr<Cell>> mocks;
    std::generate_n(std::back_inserter(mocks), 6, []() {
        auto mock = std::make_unique<CellMock>();
//        EXPECT_CALL(*mock, subscribe(testing::_)).Times(1);
        EXPECT_CALL(*mock, get_state()).WillRepeatedly(testing::Return(false));
        return std::move(mock);
    });
    EXPECT_CALL(*cell_mock_factory, make(testing::_, testing::_, testing::_, testing::_))
            .Times(6)
            .WillRepeatedly(testing::Invoke([&mocks, i = 0](bool a, int b, int c, std::__1::shared_ptr<StatePublisher> d)mutable {
                auto m = std::move(mocks.back());
                mocks.pop_back();
                i ++;
                return std::move(m);
            }));
    auto type = CellFactory::kSubscriber;
    CellFactory::registers(type, std::move(cell_mock_factory));

    auto mock = std::make_shared<StateDispatcherMock>();
    GameTest game(mock);
    int rows = 2;
    int columns = 3;

    auto config = std::make_shared<Configuration>();
    const tao::json::value v = {
            {"rows",    2},
            {"columns", 3}
    };
    config->from_json(tao::json::to_string(v));

    game.CreateGameWorld(config);
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            EXPECT_EQ(false, game.GetPixel(x, y));
        }
    }
}
