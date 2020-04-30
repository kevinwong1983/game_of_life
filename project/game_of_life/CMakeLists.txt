cmake_minimum_required(VERSION 3.13)

# projects
project(game_of_life C CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined")

add_definitions(
        -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
        -DBOOST_MPL_LIMIT_LIST_SIZE=40
        -Wno-builtin-macro-redefined)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include(InstallRedboltz.cmake)

# prepare build environment
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES})

add_dependencies(${PROJECT_NAME} redboltz)

target_link_libraries(${PROJECT_NAME}
        boost_system
        boost_date_time\${MOSQUITTO_LIBRARIES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${MOSQUITTO_INCLUDE_DIR}
        ${EXTERNAL_INSTALL_LOCATION}/include)

add_test_targets()