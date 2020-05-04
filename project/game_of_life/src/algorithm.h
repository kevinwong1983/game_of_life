/*******************************************************************************
Copyright (C) 2019 Signify Holding
All Rights Reserved.
********************************************************************************/

#pragma once

#include <functional>

// replace with std::erase_if when switch to c++20
template< typename ContainerT, typename PredicateT >
void EraseIf( ContainerT& items, const PredicateT& predicate ) {
    for( auto it = items.begin(); it != items.end(); ) {
        if( predicate(*it) ) it = items.erase(it);
        else ++it;
    }
}
