/*******************************************************************************
Copyright (C) 2019 Signify Holding
All Rights Reserved.
********************************************************************************/

#pragma once

#include <functional>

template <typename... Args>
using Callback = std::function<void(Args...)>;
