
#include "state_dispatcher_impl.h"

StateDispatcherImpl::StateDispatcherImpl(boost::asio::io_context &ioc, std::string id, std::string ip, int port)
    //: mqtt_client_(std::make_shared<MqttClient>(id, ip, port, ioc))
    {
}

void StateDispatcherImpl::on_publish(int x, int y, bool s) {
    //mqtt_client_->Publish("game_of_life/states/" + std::to_string(x) + "/" + std::to_string(y), std::to_string(s));
}