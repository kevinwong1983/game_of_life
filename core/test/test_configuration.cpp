//
// Created by user on 15/06/2020.
//

#include <iostream>
#include <gtest/gtest.h>
#include "configuration.h"
#include <tao/json.hpp>
//todo: add invalid json test "{columns:10,rows:10}"

TEST(Configuration, from_json__complete_configuration__config_set_according_to_json) {
    Configuration config;
    EXPECT_FALSE(config.rows_);
    EXPECT_FALSE(config.columns_);

    const tao::json::value v = {
            {"rows", 10},
            {"columns", 11}
    };
    auto config_json_string = tao::json::to_string(v);

    config.from_json(config_json_string);
    EXPECT_TRUE(config.rows_);
    EXPECT_TRUE(config.columns_);
    EXPECT_EQ(10, config.rows_.get());
    EXPECT_EQ(11, config.columns_.get());
}

TEST(Configuration, from_json__incomplete_configuration__part_config_set_according_to_json) {
    Configuration config;
    EXPECT_FALSE(config.rows_);
    EXPECT_FALSE(config.columns_);

    const tao::json::value v1 = {
            {"rows", 10},
    };
    auto config_json_string = tao::json::to_string(v1);

    config.from_json(config_json_string);
    EXPECT_TRUE(config.rows_);
    EXPECT_FALSE(config.columns_);
    EXPECT_EQ(10, config.rows_.get());

    const tao::json::value v2 = {
            {"columns", 11},
    };
    config_json_string = tao::json::to_string(v2);

    config.from_json(config_json_string);
    EXPECT_TRUE(config.rows_);
    EXPECT_TRUE(config.columns_);
    EXPECT_EQ(10, config.rows_.get());
    EXPECT_EQ(11, config.columns_.get());
}

TEST(Configuration, from_json__update_configuration__config_updated) {
    Configuration config;
    const tao::json::value v1 = {
            {"rows", 10},
            {"columns", 11}
    };
    auto config_json_string = tao::json::to_string(v1);
    config.from_json(config_json_string);

    const tao::json::value v2 = {
            {"rows", 20},
            {"columns", 22}
    };
    config_json_string = tao::json::to_string(v2);
    config.from_json(config_json_string);

    EXPECT_TRUE(config.rows_);
    EXPECT_TRUE(config.columns_);
    EXPECT_EQ(20, config.rows_.get());
    EXPECT_EQ(22, config.columns_.get());
}

TEST(Configuration, from_json__wrong_configuration__config_not_set) {
    Configuration config;
    const tao::json::value v1 = {
            {"rows", tao::json::null},
            {"qwe", 11}
    };
    auto config_json_string = tao::json::to_string(v1);
    config.from_json(config_json_string);

    EXPECT_FALSE(config.rows_);
    EXPECT_FALSE(config.columns_);
}

TEST(Configuration, to_json__complete_configuration__returns_complete_json) {
    Configuration config;
    config.rows_ = 10;
    config.columns_ = 11;
    auto config_json_string = config.to_json();

    auto config_json = tao::json::from_string(config_json_string);
    EXPECT_TRUE(config_json["rows"].is_unsigned());
    EXPECT_TRUE(config_json["columns"].is_unsigned());
    EXPECT_EQ(10, config_json["rows"].get_unsigned());
    EXPECT_EQ(11, config_json["columns"].get_unsigned());
}

TEST(Configuration, to_json__incomplete_configuration__returns_incomplete_json) {
    Configuration config;
    auto config_json_string = config.to_json();
    std::cout << config_json_string << std::endl;

    auto config_json = tao::json::from_string(config_json_string);
    EXPECT_TRUE(config_json["rows"].is_null());
    EXPECT_TRUE(config_json["columns"].is_null());
}