#pragma once

#include <map>
#include "cell.h"

class CellFactory {
public:
    static constexpr const auto kPublisher = "publisher";
    static constexpr const auto kSubscriber = "subscriber";

    static std::unique_ptr<Cell>
    make(const std::string &name, int x, int y, std::shared_ptr<StatePublisher> on_state_set) {
        return factory_[name]->make(false, x, y, on_state_set);
    }

    static void registers(const std::string &name, std::unique_ptr<CellItemFactory> factory) {
        factory_[name] = std::move(factory);
    }

private:
    static std::map<std::string, std::unique_ptr<CellItemFactory>> factory_;
};