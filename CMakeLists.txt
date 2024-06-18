# specify minimum cmake version
cmake_minimum_required(VERSION 3.7)
# set project name and options
project(ManySnakes
	VERSION 0.0.7.2
	DESCRIPTION "\"ManySnakes by Danielle Raine.\""
	HOMEPAGE_URL "\"https://github.com/DanielleRaine/ManySnakes\""
	LANGUAGES C
)

# specify c standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# additional warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# find SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# include SDL2 and SDL2_image
include_directories(ManySnakes
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
)

# configure project config file
configure_file(config.h.in config.h)

# add executable called ManySnakes build from source files
add_executable(ManySnakes
	main.c
	math.c
	snake.c
)

# specify where exxecutable target should look for include files
target_include_directories(ManySnakes PUBLIC
	"${PROJECT_BINARY_DIR}"
)

# link libraries
target_link_libraries(ManySnakes ${SDL2_LIBRARIES} SDL2_image::SDL2_image)

# I hate cmake, nothing makes sense
