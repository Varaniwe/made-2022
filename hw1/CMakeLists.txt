cmake_minimum_required(VERSION 3.0.0)
project(hw1 VERSION 0.1.0)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wpedantic -Wextra -Werror -fprofile-arcs -ftest-coverage -fPIC -O0")

add_subdirectory(weather)

option(USE_SANITIZER "Use sanitizer" ON)

add_executable(${PROJECT_NAME} "main.c")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov")

target_include_directories(${PROJECT_NAME} PUBLIC ${WEATHER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${WEATHER_LIBRARY})

add_subdirectory(test)