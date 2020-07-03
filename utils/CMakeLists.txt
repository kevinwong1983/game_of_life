cmake_minimum_required(VERSION 3.10.2)

# projects
project(utils C CXX)

set(CMAKE_CXX_STANDARD 14)

# prepare build environment
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(${PROJECT_NAME} ${SOURCES})

#set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

target_link_libraries(${PROJECT_NAME} PUBLIC
        -pthread)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        include/utils)