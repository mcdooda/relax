cmake_minimum_required(VERSION 2.6)

project(relax)

file(
	GLOB
	src
	src/*
	test/*
)

add_executable(
	relax
	${src}
)

target_link_libraries(
	relax
	GL
	GLU
	SDL
	expat
)
