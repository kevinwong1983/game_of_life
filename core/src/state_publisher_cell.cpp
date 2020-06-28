#include <tao/json.hpp>
#include "state_publisher_cell.h"
#include "configuration.h"

StateDispatcherCell::StateDispatcherCell(boost::asio::io_context &ioc, std::string id, std::string ip, int port)
        : mqtt_client_(std::make_shared<MosquittoMqttClient>(ip, port, id, ioc)) {
}

StateDispatcherCell::StateDispatcherCell(std::shared_ptr<MqttClient> mqtt_client)
        : mqtt_client_(mqtt_client) {
}

void StateDispatcherCell::publishConfig(std::shared_ptr<Configuration> config) {
    mqtt_client_->Publish("game_of_life/config", config->to_json(), true);
}

void StateDispatcherCell::subscribeConfig(std::function<void(std::shared_ptr<Configuration>)> callback) {
    std::cout << __func__ << " 1" << std::endl;
    mqtt_client_->Subscribe("game_of_life/config", [&callback](const Topic &t, const Message &m) {
        (void) t;
        std::cout << __func__ << " 2" << std::endl;
        auto c = std::make_shared<Configuration>();
        c->from_json(m);
        if (c->rows_ && c->columns_) {
            std::cout << __func__ << " 3" << std::endl;
            callback(c);
        }
    });
}

void StateDispatcherCell::on_publish(int x, int y, bool s) {
    mqtt_client_->Publish("game_of_life/states/" + std::to_string(x) + "/" + std::to_string(y), std::to_string(s),
                          true);
}

boost::signals2::connection
StateDispatcherCell::subscribe(int x, int y, std::function<void(const Topic &t, const Message &m)> callback) {
    return mqtt_client_->Subscribe("game_of_life/states/" + std::to_string(x) + "/" + std::to_string(y), callback);
}
