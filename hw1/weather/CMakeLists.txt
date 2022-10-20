cmake_minimum_required(VERSION 3.0.0)
project(weather_lib VERSION 0.1.0)

file(GLOB SOURCES "src/*.c")
file(GLOB INCLUDE_DIRS "include")
file(GLOB HEADERS "include/*.h")

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

set(WEATHER_LIBRARY ${PROJECT_NAME} PARENT_SCOPE)
set(WEATHER_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)