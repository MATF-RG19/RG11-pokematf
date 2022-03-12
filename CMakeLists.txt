cmake_minimum_required(VERSION 3.16)
project(Pokemon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglut -lGLU -lGL -lm")


add_library(
	image
	image.cpp
	image.h
)

add_library(
	pokematf
	pokematf.cpp
	pokematf.h
)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE image
					       pokematf
)


