#pragma once
#include <boost/signals2.hpp>
#include "topic.h"
#include "message.h"
#include "configuration.h"
#include "state_publisher_abstract.h"

class StatePublisher : public StatePublisherAbstract {
public:
    virtual void publishConfig(std::shared_ptr<Configuration> config) = 0;
    virtual void subscribeConfig(std::function<void(std::shared_ptr<Configuration>)> callback) = 0;
    virtual boost::signals2::connection subscribe(int x, int y, std::function<void(const Topic &t, const Message &m)> callback) = 0;
};
