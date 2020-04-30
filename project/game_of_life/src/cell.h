#pragma once
#include <boost/signals2.hpp>
#include "markable.h"
#include "publishable_with_context.h"

class Cell : public Markable{
public:
    Cell(bool state, int x, int y);
    void mark_state(bool mark) override;
    void update() override;
    bool get_state() const override;
    void set_state(bool state) override;
    void subscribe(std::shared_ptr<PublishableWithContext> subscriber);
    int get_x() const;
    int get_y() const;
private:
    int x_;
    int y_;
    bool marked_ = false;
    bool state_ = false;
    boost::signals2::signal<void(int x, int y, bool s)> state_publisher_;
};