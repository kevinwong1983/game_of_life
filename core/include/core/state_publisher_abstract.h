#pragma once

class StatePublisherAbstract {
public:
    virtual void on_publish(int x, int y, bool s) = 0;
};
