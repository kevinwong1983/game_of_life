#include <iostream>
#include <mosquitto.h>
#include <mutex>
#include <string>

#include "mosquitto_mqtt_client.h"

std::mutex MosquittoMqttClient::lib_init_mutex;
int MosquittoMqttClient::instances;

MosquittoMqttClient::MosquittoMqttClient(
        const std::string& ip_address,
        unsigned short port,
        const std::string& client_id,
        boost::asio::io_context& ioc) : ioc_(ioc) {
    if (instances == 0) {
        std::lock_guard<std::mutex> guard (lib_init_mutex);
        mosquitto_lib_init();
    }
    instances++;

    auto c = mosquitto_new(client_id.c_str(), true, this);
    if (!c) {
        throw std::runtime_error("Could not create new instance of mosquitto client");
    }

    client_.reset(c, [](mosquitto* cl) { mosquitto_destroy(cl); });
    mosquitto_int_option(client_.get(), MOSQ_OPT_PROTOCOL_VERSION, MQTT_PROTOCOL_V311);
    mosquitto_message_callback_set(client_.get(), OnPublish);
    mosquitto_connect_callback_set(client_.get(), OnConnect);
    mosquitto_connect (client_.get(), ip_address.c_str(), port, 60);
    mosquitto_loop_start(client_.get());
}

MosquittoMqttClient::~MosquittoMqttClient() {
    Disconnect();
    mosquitto_loop_stop(client_.get(), false);

    --instances;
    if (instances == 0) {
        mosquitto_lib_cleanup();
    }
}

void MosquittoMqttClient::OnPublish(struct mosquitto* mosq, void* obj, const mosquitto_message* msg) {
    auto self = static_cast<MosquittoMqttClient*>(obj);

    for (const auto& subscription : self->subscriptions_) {
        bool match;
        mosquitto_topic_matches_sub(subscription.first.c_str(), msg->topic, &match);

        if (match) {
            auto m = payload_to_string(msg->payload, msg->payloadlen);
            auto t = std::string(msg->topic);
            self->ioc_.post([&subscription, m, t]{
                subscription.second(t, m);
            });

            break;
        }
    }
}

std::string MosquittoMqttClient::payload_to_string(void* message, int length){
    char* a = static_cast<char*>(message);
    char b[length + 1];
    b[length] = '\0';
    int i = 0;
    while (i < length) {
        b[i] = a[i];
        i++;
    }
    return std::string(b);
}

void MosquittoMqttClient::OnConnect(mosquitto* mosq, void* obj, int rc){
    auto self = static_cast<MosquittoMqttClient*>(obj);
    std::cout << __func__ << std::endl;
}

Connection MosquittoMqttClient::Subscribe(const Topic& topic, Sink<const Topic&, const Message&> sink) {
    mosquitto_subscribe(client_.get(), NULL, topic.c_str(), 0);
    auto connection = subscriptions_[topic].connect(sink);
    return connection;
}

void MosquittoMqttClient::Unsubscribe(const Topic& topic) {
    mosquitto_unsubscribe(client_.get(), NULL, topic.c_str());
    subscriptions_.erase(topic);
}

void MosquittoMqttClient::Publish(const Topic& topic, const Message& message) {
    mosquitto_publish(client_.get(), NULL, topic.c_str(), message.size(), message.c_str(), 1, false);
}

void MosquittoMqttClient::Disconnect() {
    mosquitto_disconnect(client_.get());
}