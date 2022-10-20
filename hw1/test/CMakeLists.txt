cmake_minimum_required(VERSION 3.0.0)
project(test_hw1)
enable_testing()

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES *.cpp)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${WEATHER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${WEATHER_LIBRARY} GTest::gtest_main)

add_test(test_name test_hw1)
add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND ctest --output-on-failure)