cmake_minimum_required(VERSION 3.13)

# projects
project(game_of_life C CXX)

set(CMAKE_CXX_STANDARD 14)

# prepare build environment
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES})

message(STATUS "MOSQUITTO_LIBRARIES: ${MOSQUITTO_LIBRARIES}")
message(STATUS "MOSQUITTO_INCLUDE_DIR: ${MOSQUITTO_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME}
        boost_system
        boost_date_time
        -pthread
        ${MOSQUITTO_LIBRARIES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${MOSQUITTO_INCLUDE_DIR})

add_test_targets()