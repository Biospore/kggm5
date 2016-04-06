cmake_minimum_required(VERSION 2.8)
project(go)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
#find_package(GLM REQUIRED)


set(SOURCE_FILES main_old.cpp)
add_executable(go ${SOURCE_FILES})

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(go ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})