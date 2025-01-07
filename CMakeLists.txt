cmake_minimum_required(VERSION 3.15...3.31)

project(ManySnakes
	VERSION 0.0.12.2
	DESCRIPTION "ManySnakes by Danielle Raine. There are... many snakes."
	HOMEPAGE_URL "https://github.com/DanielleRaine/ManySnakes"
	LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Lua REQUIRED)

configure_file("${CMAKE_SOURCE_DIR}/src/version.h.in" "${CMAKE_BINARY_DIR}/version.h")

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

add_executable("${CMAKE_PROJECT_NAME}"
	src/main.c
	src/snake.c
	src/texture.c
	src/resource.c)

target_include_directories(ManySnakes PRIVATE "${PROJECT_BINARY_DIR}")

target_link_libraries(ManySnakes SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf "${LUA_LIBRARIES}")

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${PROJECT_SOURCE_DIR}/assets" "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/assets")

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${PROJECT_SOURCE_DIR}/scripts" "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/scripts")
