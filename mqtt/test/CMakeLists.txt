cmake_minimum_required(VERSION 3.12.3)

file(GLOB_RECURSE SOURCES
        "*.cpp"
        "../src/*.cpp"
)

package_add_test(${PROJECT_NAME}_unit_tests ${SOURCES})

target_link_libraries(${PROJECT_NAME}_unit_tests
        boost_system
        boost_date_time
        mosquitto)

target_include_directories(${PROJECT_NAME}_unit_tests
        PUBLIC
        ../include/mqtt
)