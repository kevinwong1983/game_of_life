#pragma once

#include <gmock/gmock.h>
#include "../../src/state_dispatcher.h"

class StateDispatcherMock : public StateDispatcher {
public:
    MOCK_METHOD3(on_publish, void(int x, int y, bool s));
};