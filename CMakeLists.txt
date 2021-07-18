cmake_minimum_required(VERSION 3.19)
project(cpp_falling_sand)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(external)

add_executable(cpp_falling_sand app/main.cpp)
target_link_libraries(cpp_falling_sand glfw OpenGL::GL GLEW::GLEW glm::glm stb_image)
target_include_directories(cpp_falling_sand PRIVATE include)

add_custom_command(TARGET cpp_falling_sand POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        )