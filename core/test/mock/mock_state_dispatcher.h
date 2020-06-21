#pragma once

#include <gmock/gmock.h>
#include "state_publisher.h"

class StateDispatcherMock : public StatePublisher {
public:
    MOCK_METHOD1(publishConfig, void(std::shared_ptr<Configuration> config));
    MOCK_METHOD1(subscribeConfig, void(std::function<void(std::shared_ptr<Configuration>)> callback));
    MOCK_METHOD3(subscribe, boost::signals2::connection(int x, int y, std::function<void(const Topic &t, const Message &m)> callback));
    MOCK_METHOD3(on_publish, void(int x, int y, bool s));
};