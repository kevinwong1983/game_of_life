#pragma once

class Markable {
    virtual void mark_state(bool mark) = 0;
    virtual void update() = 0;
    virtual bool get_state() const = 0;
    virtual void set_state(bool state) = 0;
};
