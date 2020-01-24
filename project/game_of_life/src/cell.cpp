#include "cell.h"

Cell::Cell(bool state):state_(state), marked_for_state_change_(false){
}

bool Cell::getState() const {
    return state_;
}

void Cell::setState(bool state){
    state_ = state;
}

void Cell::markForStateChange(bool state){
    marked_for_state_change_ = true;
    state_change_ = state;
}

void Cell::applyStateChange(){
    if(marked_for_state_change_){
        state_ = state_change_;
    }
    marked_for_state_change_ = false;
}