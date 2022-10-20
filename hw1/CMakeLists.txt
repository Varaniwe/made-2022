cmake_minimum_required(VERSION 3.0.0)
project(hw1 VERSION 0.1.0)

set(USE_SANITIZER 0)
if(USE_SANITIZER)
set(SANITIZER_FLAGS ${SANITIZER_FLAGS} "-fsanitize=address,undefined -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error")
else()
set(SANITIZER_FLAGS ${SANITIZER_FLAGS} "")
endif()

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wpedantic -Wextra -Werror -fprofile-arcs -ftest-coverage -fPIC -O0 ${SANITIZER_FLAGS}")

add_subdirectory(weather)

add_executable(${PROJECT_NAME} "main.c")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov")

target_include_directories(${PROJECT_NAME} PUBLIC ${WEATHER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${WEATHER_LIBRARY} ${SANITIZER_FLAGS})

add_subdirectory(test)