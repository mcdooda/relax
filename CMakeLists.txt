cmake_minimum_required(VERSION 2.6)

project(relax)

file(
	GLOB
	src
	src/*
	test/*
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -std=c++11")

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
	expat
	lua5.2
)
