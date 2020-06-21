cmake_minimum_required(VERSION 3.12.3)

file(GLOB_RECURSE SOURCES
        "*.cpp"
        "mock/*.cpp"
        "../src/*.cpp"
)
list(REMOVE_ITEM SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp)

package_add_test(${PROJECT_NAME}_unit_tests ${SOURCES})

target_link_libraries(${PROJECT_NAME}_unit_tests
        boost_system
        boost_date_time
        core
        mqtt_lib
        mosquitto)

target_include_directories(${PROJECT_NAME}_unit_tests
        PUBLIC
        ../include/game)