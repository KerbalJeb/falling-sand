cmake_minimum_required(VERSION 3.19)
project(cpp_falling_sand)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(cpp_falling_sand app/main.cpp)
target_link_libraries(cpp_falling_sand glfw OpenGL::GL GLEW::GLEW)