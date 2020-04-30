#include "mqtt_publish_options.h"

mqtt::publish_options MqttPublishOptions::get() const {
    MQTT_NS::qos qos = MQTT_NS::qos::at_least_once;
    MQTT_NS::retain retain = MQTT_NS::retain::no;

    if (QoS) {
        qos = static_cast<MQTT_NS::qos>(*QoS);
    }
    if (Retain) {
        retain = static_cast<MQTT_NS::retain>(*Retain);
    }
    mqtt::publish_options rv = qos | retain;
    return rv;
};