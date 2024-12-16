cmake_minimum_required(VERSION 3.15...3.31)

project(ManySnakes
	VERSION 0.0.12.2
	DESCRIPTION "ManySnakes by Danielle Raine. There are... many snakes."
	LANGUAGES C)

add_executable(ManySnakes src/main.c src/snake.c src/texture.c)


