project("game of life")
cmake_minimum_required(VERSION 3.10.2)

include(cmake/utilities.cmake)
include(cmake/FindMosquitto.cmake)
include(cmake/FindTaoJson.cmake)
include(cmake/FindClangTidy.cmake)
include(CMakePrintHelpers)
# build options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# prepare build environment

# 3rd party dependencies
find_package(Boost REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_mosquitto()

# creates a make test target
enable_testing()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# projects
add_subdirectory(mqtt)
add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(viewer)
add_subdirectory(utils)