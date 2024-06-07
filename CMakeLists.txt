cmake_minimum_required(VERSION 3.5)

project(ManySnakes VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules/ ${CMAKE_MODULE_PATH})

find_package(SDL REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(ManySnakes ${SDL2_LIBRARY})

add_executable(ManySnakes main.c)

