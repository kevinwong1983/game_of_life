#pragma once
#include "state_publisher.h"

class Cell {
public:
    virtual ~Cell() = default;

    virtual void mark_state(bool mark) = 0;

    virtual void update() = 0;

    virtual bool get_state() const = 0;

    virtual void set_state(bool state) = 0;

    virtual int get_x() const = 0;

    virtual int get_y() const = 0;

    virtual void subscribe(std::shared_ptr<StatePublisherAbstract> on_state_set) = 0;
};

class CellItemFactory {
public:
    virtual ~CellItemFactory() = default;
    virtual std::unique_ptr<Cell> make(bool state, int x, int y, std::shared_ptr<StatePublisher> on_state_set) = 0;
};