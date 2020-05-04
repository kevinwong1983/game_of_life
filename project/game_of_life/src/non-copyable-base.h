/*******************************************************************************
 Copyright (C) 2019 Signify Holding
 All Rights Reserved.
 ********************************************************************************/

#pragma once

class NonCopyableBase {
public:
    NonCopyableBase(const NonCopyableBase&) = delete;
    NonCopyableBase(NonCopyableBase&&) = delete;

    NonCopyableBase& operator=(const NonCopyableBase&) = delete;
    NonCopyableBase& operator=(NonCopyableBase&&) = delete;

protected:
    NonCopyableBase() = default;
    virtual ~NonCopyableBase() = default;
};