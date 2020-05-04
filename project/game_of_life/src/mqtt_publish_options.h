#pragma once

#include <boost/optional.hpp>

enum class QoSLevel {
    zero = 0,
    one = 1,
    two = 2,
};

class MqttPublishOptions {
public:
    boost::optional<QoSLevel> QoS;
    boost::optional<bool> Retain;
};