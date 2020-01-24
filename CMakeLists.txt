cmake_minimum_required(VERSION 3.10)

include(utilities.cmake)

# build options

# prepare build environment

# 3rd party dependencies
find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

# projects
add_subdirectory(project)