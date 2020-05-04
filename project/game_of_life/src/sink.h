/*******************************************************************************
Copyright (C) 2019 Signify Holding
All Rights Reserved.
********************************************************************************/

#pragma once

#include "callback.h"
#include "connection.h"

template <typename... Args>
using Sink = Callback<Args...>;
