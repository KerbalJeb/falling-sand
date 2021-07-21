cmake_minimum_required(VERSION 3.19)
project(cpp_falling_sand)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Catch2 REQUIRED)

add_subdirectory(external)

add_executable(cpp_falling_sand app/main.cpp
        lib/simulation/brushes.cpp
        lib/simulation/element.cpp
        lib/simulation/element_manager.cpp
        lib/simulation/element_update.cpp
        lib/simulation/rn_generator.cpp
        lib/simulation/simulation_canvas.cpp)

target_link_libraries(cpp_falling_sand glfw OpenGL::GL GLEW::GLEW glm::glm stb_image)
target_include_directories(cpp_falling_sand PRIVATE include app)

add_executable(test_util test/test_util.cpp)
target_link_libraries(test_util Catch2::Catch2 stb_image)
target_include_directories(test_util PRIVATE include)

add_custom_command(TARGET cpp_falling_sand POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
        )