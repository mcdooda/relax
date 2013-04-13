cmake_minimum_required(VERSION 2.6)

project(relax)

file(
	GLOB_RECURSE
	src
	src/*
	test/*
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -ansi")

add_executable(
	relax
	${src}
)

target_link_libraries(
	relax
	GL
	GLU
	SDL
	SDL_image
	SDL_ttf
	expat
	lua5.2
)
