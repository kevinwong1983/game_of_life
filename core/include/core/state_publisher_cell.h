#pragma once

#include <boost/asio.hpp>
#include "state_publisher.h"
#include "mosquitto_mqtt_client.h"

class StateDispatcherCell : public StatePublisher {
public:
    StateDispatcherCell(boost::asio::io_context &ioc, std::string id, std::string ip, int port);

    StateDispatcherCell(std::shared_ptr<MqttClient> mqtt_client);

    void publishConfig(std::shared_ptr<Configuration> config) override;

    void subscribeConfig(std::function<void(std::shared_ptr<Configuration>)> callback) override;

    void on_publish(int x, int y, bool s) override;

    boost::signals2::connection
    subscribe(int x, int y, std::function<void(const Topic &t, const Message &m)> callback) override;

private:
    std::shared_ptr<MqttClient> mqtt_client_;
};