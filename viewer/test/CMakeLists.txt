cmake_minimum_required(VERSION 3.12.3)

file(GLOB_RECURSE SOURCES
        "*.cpp"
        "mock/*.cpp"
        "../src/*.cpp"
        "../src/view/*.cpp"
        "../src/view/*.h"
        "../src/model/*.cpp"
        "../src/model/*.h"
)
list(REMOVE_ITEM SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp)

package_add_test(${PROJECT_NAME}_unit_tests ${SOURCES})

target_link_libraries(${PROJECT_NAME}_unit_tests
        Qt5::Widgets
        boost_system
        boost_date_time
        core
        mqtt_lib
        mosquitto
        taocpp::json)

target_include_directories(${PROJECT_NAME}_unit_tests
        PRIVATE
        ../src/model
        ../src/view
        PUBLIC
        ../include/viewer)