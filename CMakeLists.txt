cmake_minimum_required(VERSION 3.16)
project(Pokemon)

set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


add_executable(${PROJECT_NAME} main.cpp pokematf.cpp image.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})


