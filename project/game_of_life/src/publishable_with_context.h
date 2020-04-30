#pragma once

class PublishableWithContext {
public:
    virtual void on_publish(int x, int y, bool s) = 0;
};
