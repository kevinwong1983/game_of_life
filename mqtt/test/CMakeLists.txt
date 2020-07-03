cmake_minimum_required(VERSION 3.10.2)

file(GLOB_RECURSE SOURCES
        "*.cpp"
        "../src/*.cpp"
)

package_add_test(${PROJECT_NAME}_unit_tests ${SOURCES})

target_link_libraries(${PROJECT_NAME}_unit_tests
        boost_system
        boost_date_time
        mosquitto
        core)

target_include_directories(${PROJECT_NAME}_unit_tests
        PRIVATE
        ../include/mqtt
)