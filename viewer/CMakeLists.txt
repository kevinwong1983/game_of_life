cmake_minimum_required(VERSION 3.12.3)

# projects
project(viewer C CXX)

# qt cmake config
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)

set(Qt5_DIR "~/Qt5.14.2/5.14.2/clang_64/lib/cmake/Qt5") # todo: dir should be set automatically by find_package
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

# prepare build environment
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*/*.cpp" "src/*/*.h"  "src/*/*.ui")
message(STATUS SOURCES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt5::Widgets
        Qt5::Network
        PUBLIC
        -pthread
        core
        boost_system
        boost_date_time
        boost_program_options
        mqtt_lib
        mosquitto)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        src/model
        src/view
        PUBLIC
        include/viewer)

add_subdirectory(test)