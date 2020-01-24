#include <iostream>
#include <gtest/gtest.h>
#include "cell.h"

using namespace std;

TEST(cell_test, constructor_sets_state) {
    Cell c(true);
    EXPECT_EQ(true, c.getState());
    Cell c2(false);
    EXPECT_EQ(false, c2.getState());
}

TEST(cell_test, set_state_get_state) {
    Cell c(false);
    c.setState(true);
    EXPECT_EQ(true, c.getState());
    c.setState(false);
    EXPECT_EQ(false, c.getState());
}

TEST(cell_test, mark_and_apply) {
    Cell c(false);
    c.markForStateChange(true);
    EXPECT_EQ(false, c.getState());

    c.applyStateChange();
    EXPECT_EQ(true, c.getState());

    c.markForStateChange(false);
    EXPECT_EQ(true, c.getState());

    c.applyStateChange();
    EXPECT_EQ(false, c.getState());
}

