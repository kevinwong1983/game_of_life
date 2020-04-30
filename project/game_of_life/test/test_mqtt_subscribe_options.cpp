#include "boost/variant.hpp"
#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <time.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/system_error.hpp>
#include <functional>
#include <unordered_map>

#include "mqtt_publish_options.h"
#include <mqtt_client_cpp.hpp>

TEST(mqtt_options, mqttPublishOptions) {
    MqttPublishOptions options;

    options.QoS = QoSLevel::zero;
    options.Retain = false;
    mqtt::publish_options po = options.get();
    EXPECT_EQ(0, (std::uint8_t)po.get_retain());
    EXPECT_EQ(0, (std::uint8_t)po.get_qos());

    options.QoS = QoSLevel::one;
    options.Retain = false;
    po = options.get();
    EXPECT_EQ(0, (std::uint8_t)po.get_retain());
    EXPECT_EQ(1, (std::uint8_t)po.get_qos());

    options.QoS = QoSLevel::two;
    options.Retain = false;
    po = options.get();
    EXPECT_EQ(0, (std::uint8_t)po.get_retain());
    EXPECT_EQ(2, (std::uint8_t)po.get_qos());

    options.QoS = QoSLevel::zero;
    options.Retain = true;
    po = options.get();
    EXPECT_EQ(1, (std::uint8_t)po.get_retain());
    EXPECT_EQ(0, (std::uint8_t)po.get_qos());

    options.QoS = QoSLevel::one;
    options.Retain = true;
    po = options.get();
    EXPECT_EQ(1, (std::uint8_t)po.get_retain());
    EXPECT_EQ(1, (std::uint8_t)po.get_qos());

    options.QoS = QoSLevel::two;
    options.Retain = true;
    po = options.get();
    EXPECT_EQ(1, (std::uint8_t)po.get_retain());
    EXPECT_EQ(2, (std::uint8_t)po.get_qos());
}