project("game of life")
cmake_minimum_required(VERSION 3.10)

include(utilities.cmake)

# build options

# prepare build environment

# 3rd party dependencies
find_package(Boost REQUIRED)
message(STATUS "libboost: ${BOOST_INCLUDE_DIRS} ${BOOST_LIBRARIES}")
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Find mosquitto
include(findmosquitto.cmake)
find_mosquitto()

# projects
add_subdirectory(project)