#pragma once

#include <boost/signals2.hpp>
#include "cell.h"
#include "state_publisher_abstract.h"
#include "state_publisher.h"

class CellSubscriber : public Cell {
public:
    CellSubscriber(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set);

    ~CellSubscriber() {};

    void mark_state(bool mark) override;

    void update() override;

    bool get_state() const override;

    void set_state(bool state) override;

    int get_x() const override;

    int get_y() const override;

    void subscribe(std::shared_ptr<StatePublisherAbstract> on_state_set) override;

private:
    void subscribeHandler(const Topic &t, const Message &m);

    int x_;
    int y_;
    bool marked_;
    bool state_;
    boost::signals2::scoped_connection sc_;
    boost::signals2::signal<void(int x, int y, bool s)> notify_state_change_;
};

class CellSubscriberFactory : public CellItemFactory {
public:
    std::unique_ptr<Cell> make(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set) override {
        return std::make_unique<CellSubscriber>(state, x, y, on_state_set);
    }
};