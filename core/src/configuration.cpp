//
// Created by user on 15/06/2020.
//
#include <tao/json.hpp>
#include <iostream>
#include "configuration.h"

Configuration::Configuration() : rows_(boost::none), columns_(boost::none) {
}

void Configuration::from_json(std::string json_string) {
    std::cout << __func__ << json_string << std::endl;
    auto json = tao::json::from_string(json_string);

    if (json["rows"].is_unsigned()) {
        rows_ = json["rows"].get_unsigned();
    }
    if (json["columns"].is_unsigned()) {
        columns_ = json["columns"].get_unsigned();
    }
}

std::string Configuration::to_json() {
    tao::json::value v = tao::json::empty_object;
    if (rows_) {
        v["rows"] = rows_.get();
    }
    else {
        v["rows"] = tao::json::null;
    }
    if (columns_) {
        v["columns"] = columns_.get();
    }
    else {
        v["columns"] = tao::json::null;
    }
    return tao::json::to_string(v);
}