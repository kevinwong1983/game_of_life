/*******************************************************************************
  Copyright (C) 2019 Signify Holding
  All Rights Reserved.
********************************************************************************/

#pragma once
#include <iostream>
#include "non-copyable-base.h"
#include "connection.h"
#include "sink.h"
#include "topic.h"
#include "message.h"

class MqttClient : public NonCopyableBase {
public:
    virtual Connection Subscribe(const Topic& topic, Sink<const Topic&, const Message&> sink) = 0;
    virtual void Unsubscribe(const Topic& topic) = 0;
    virtual void Publish(const Topic& topic, const Message& message, const bool retain = false) = 0;
    virtual void Disconnect() = 0;
};