#pragma once

#include <boost/asio.hpp>
#include "publishable_with_context.h"
#include "mqtt_client.h"

class StateDispatcher : public PublishableWithContext {
public:
    virtual void on_publish(int x, int y, bool s) = 0;
};