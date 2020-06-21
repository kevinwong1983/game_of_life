#pragma once

#include <string>
#include <boost/optional.hpp>

class Configuration {
public:
    boost::optional<int> rows_;
    boost::optional<int> columns_;

    Configuration();
    void from_json(std::string json);
    std::string to_json();
};
