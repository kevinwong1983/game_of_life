//
// Created by user on 13/06/2020.
//

#include "cell_factory.h"
#include "cell_publisher.h"
#include "cell_subscriber.h"

static std::map<std::string, std::unique_ptr<CellItemFactory>> init() {
    std::map<std::string, std::unique_ptr<CellItemFactory>> map;
    map[CellFactory::kPublisher] = std::move(std::make_unique<CellPublisherFactory>());
    map[CellFactory::kSubscriber] = std::move(std::make_unique<CellSubscriberFactory>());
    return map;
}

std::map<std::string, std::unique_ptr<CellItemFactory>> CellFactory::factory_ = init();