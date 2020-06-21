#pragma once

#include <gmock/gmock.h>
#include "state_publisher.h"

class StateDispatcherAbstactMock : public StatePublisherAbstract {
public:
    MOCK_METHOD3(on_publish, void(int x, int y, bool s));
};