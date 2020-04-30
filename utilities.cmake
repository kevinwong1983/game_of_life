function(add_test_targets)
    cmake_minimum_required(VERSION 3.10)
    include(GoogleTest)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
        set(TEST_PROJECT_NAME ${PROJECT_NAME}_unit_tests)
        add_subdirectory(
                test
                test/mock)
    endif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
endfunction(add_test_targets)

macro(add_test_project)
    project(${TEST_PROJECT_NAME} C CXX)
#    set(SOURCES ${LIBRARY_NAMES_LIST} ${ARGN})
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PRIVATE dl -pthread GTest::GTest gmock GTest::Main boost_system boost_date_time)
    gtest_discover_tests(${PROJECT_NAME} TEST_PREFIX "${PROJECT_NAME}::")
    message(STATUS "${PROJECT_NAME} target added.")
endmacro(add_test_project)