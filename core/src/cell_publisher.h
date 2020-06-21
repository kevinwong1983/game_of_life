#pragma once

#include <memory>
#include <boost/signals2.hpp>
#include "cell.h"
#include "state_publisher.h"

class CellPublisher : public Cell {
public:
    CellPublisher(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set);

    CellPublisher(const CellPublisher &that);

    CellPublisher &operator=(const CellPublisher &that);

    ~CellPublisher();

    void mark_state(bool mark) override;

    void update() override;

    bool get_state() const override;

    void set_state(bool state) override;

    int get_x() const override;

    int get_y() const override;

    void subscribe(std::shared_ptr<StatePublisherAbstract> on_state_set) override;

private:
    void subscribeToStateSet(std::shared_ptr<StatePublisher> on_state_set);

    int x_;
    int y_;
    bool marked_ = false;
    bool state_ = false;
    boost::signals2::signal<void(int x, int y, bool s)> notify_state_change_;
};

struct CellPublisherFactory : public CellItemFactory {
public:
    std::unique_ptr<Cell> make(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set) override {
        return std::make_unique<CellPublisher>(state, x, y, on_state_set);
    }
};