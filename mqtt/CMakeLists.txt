cmake_minimum_required(VERSION 3.10.2)

# projects
project(mqtt_lib C CXX)

set(CMAKE_CXX_STANDARD 14)

# prepare build environment
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        -pthread
        boost_system
        boost_date_time
        core
        utils
        mosquitto)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        include/utils
        include/core
        PUBLIC
        include/mqtt)

add_subdirectory(test)