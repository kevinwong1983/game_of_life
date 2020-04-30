#pragma once

#include <boost/asio.hpp>
#include "state_dispatcher.h"
#include "mqtt_client.h"

class StateDispatcherImpl : public StateDispatcher {
public:
    StateDispatcherImpl(boost::asio::io_context &ioc, std::string id, std::string ip, int port);
    void on_publish(int x, int y, bool s) override;
private:
    std::shared_ptr<MqttClient> mqtt_client_;
};