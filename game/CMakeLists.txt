cmake_minimum_required(VERSION 3.12.3)

# projects
project(game C CXX)

set(CMAKE_CXX_STANDARD 14)

# prepare build environment
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
        -pthread
        core
        boost_system
        boost_date_time
        boost_program_options
        mqtt_lib
        mosquitto)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        include/game)

add_subdirectory(test)