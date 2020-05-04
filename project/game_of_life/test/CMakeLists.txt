cmake_minimum_required(VERSION 3.13)

file(GLOB_RECURSE SOURCES "*.cpp" "../src/*.cpp")
list(REMOVE_ITEM SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp"
        )

add_test_project()
target_include_directories(${TEST_PROJECT_NAME}
        PUBLIC
        ${EXTERNAL_INSTALL_LOCATION}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        )