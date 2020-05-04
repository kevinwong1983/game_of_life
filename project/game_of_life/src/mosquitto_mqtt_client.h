#pragma once

#include <boost/asio/io_context.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <boost/signals2/signal.hpp>

#include "mqtt_client.h"

struct mosquitto;
struct mosquitto_message;

// -> move them in separate header files
class MqttError {
public:
    MqttError(int code);
    int code();
    const char* toString();

private:
    int code_;
};

class MqttException : public std::exception {
public:
    MqttException(MqttError code) : code_(code){};
    const char* what() { return code_.toString(); }

    MqttError code() { return code_; }

private:
    MqttError code_;
};
// <- move them in separate header files

class MosquittoMqttClient : public MqttClient {
    using Subscriptions = std::map<Topic, boost::signals2::signal<void(const Topic&, const Message&)>>;

public:
    MosquittoMqttClient(
            const std::string& ip_address, unsigned short port, const std::string& client_id, boost::asio::io_context& ioc);
    ~MosquittoMqttClient();

    Connection Subscribe(const Topic& topic, Sink<const Topic&, const Message&> sink) override;
    void Unsubscribe(const Topic& topic) override;
    void Publish(const Topic& topic, const Message& message) override;
    void Disconnect() override;

    const Subscriptions& subscriptions() const;

private:
    static std::string payload_to_string(void* message, int length);
    static void OnPublish(mosquitto* mosq, void* obj, const mosquitto_message* msg);
    static void OnConnect(mosquitto* mosq, void* obj, int rc);

    std::map<Topic, boost::signals2::signal<void(const Topic&, const Message&)>> subscriptions_;
    std::shared_ptr<mosquitto> client_;
    boost::asio::io_context& ioc_;

    static std::mutex lib_init_mutex;  // protects g_i
    static int instances;
};