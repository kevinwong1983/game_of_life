#pragma once

#include <gmock/gmock.h>
#include "cell.h"

class CellMock : public Cell {
public:
    MOCK_METHOD1(mark_state, void(bool mark));
    MOCK_METHOD0(update, void());
    MOCK_CONST_METHOD0(get_state, bool());
    MOCK_METHOD1(set_state, void(bool state));
    MOCK_CONST_METHOD0(get_x, int());
    MOCK_CONST_METHOD0(get_y, int());
    MOCK_METHOD1(subscribe, void(std::shared_ptr<StatePublisherAbstract> on_state_set));
};

struct CellMockFactory : public CellItemFactory {
    MOCK_METHOD4(make, std::unique_ptr<Cell>(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set));
};