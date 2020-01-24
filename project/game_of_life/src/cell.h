#pragma once

class Cell {
public:
    Cell(bool state);
    bool getState() const;
    void setState(bool state);
    void markForStateChange(bool state);
    void applyStateChange();

private:
    bool state_;
    bool marked_for_state_change_;
    bool state_change_;
};