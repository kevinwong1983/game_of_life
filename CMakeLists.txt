project("game of life")
cmake_minimum_required(VERSION 3.12.3)

include(cmake/utilities.cmake)
include(cmake/FindMosquitto.cmake)
include(cmake/FindTaoJson.cmake)
include(CMakePrintHelpers)
# build options

# prepare build environment

# 3rd party dependencies
find_package(Boost REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_mosquitto()

# projects
add_subdirectory(mqtt)
add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(viewer)