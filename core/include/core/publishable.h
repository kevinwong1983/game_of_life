#pragma once

class Publishable {
public:
    virtual ~Publishable() = default;
    virtual void on_publish() = 0;
};
