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
    }
};

TEST(Game, CreateGameWorld__ValidConfiguration__PixelSetToFalse) {
    int rows = 2;
    int columns = 3;

    auto cell_mock_factory = std::make_unique<CellMockFactory>();
    std::vector<std::unique_ptr<Cell>> mocks;
    std::generate_n(std::back_inserter(mocks), rows * columns, []() {
        auto mock = std::make_unique<CellMock>();
//        EXPECT_CALL(*mock, subscribe(testing::_)).Times(1);
        EXPECT_CALL(*mock, get_state()).WillRepeatedly(testing::Return(false));
        return mock;
    });
    EXPECT_CALL(*cell_mock_factory, make(testing::_, testing::_, testing::_, testing::_))
            .Times(rows * columns)
            .WillRepeatedly(testing::Invoke(
                     [&mocks](bool a, int b, int c, std::shared_ptr<StatePublisher> d) {
                         (void) a;
                         (void) b;
                         (void) c;
                         (void) d;
                        auto m = std::move(mocks.back());
                        mocks.pop_back();
                        return m;
                    }));
    auto type = CellFactory::kSubscriber;
    CellFactory::registers(type, std::move(cell_mock_factory));

    auto mock = std::make_shared<StateDispatcherMock>();
    GameTest game(mock);

    auto config = std::make_shared<Configuration>();
    const tao::json::value v = {
            {"rows",    2},
            {"columns", 3}
    };
    config->from_json(tao::json::to_string(v));

    game.CreateGame(config);
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            EXPECT_EQ(false, game.GetPixelState(x, y));
        }
    }
}
