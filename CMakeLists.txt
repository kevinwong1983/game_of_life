project("game of life")
cmake_minimum_required(VERSION 3.10)

include(utilities.cmake)
include(findmosquitto.cmake)

# build options

# prepare build environment

# 3rd party dependencies
find_package(Boost REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_mosquitto()

# projects
add_subdirectory(project)