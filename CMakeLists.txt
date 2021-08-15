cmake_minimum_required(VERSION 3.18)
project(cpp_falling_sand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm)
find_package(Catch2 REQUIRED)

include(FetchContent)
if (NOT glm_FOUND)
    FetchContent_Declare(glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG "0.9.9.8"
            )
    FetchContent_MakeAvailable(glm)
endif ()

add_subdirectory(external)

add_executable(cpp_falling_sand app/main.cpp
        src/simulation/brushes.cpp
        src/simulation/element.cpp
        src/simulation/element_manager.cpp
        src/simulation/rn_generator.cpp
        src/simulation/simulation_canvas.cpp
        src/graphics/gl/shader.cpp
        src/graphics/gl/texture2d.cpp
        src/graphics/gl/vertex_array.cpp
        src/graphics/gl/vertex_buffer.cpp
        src/graphics/image.cpp
        src/graphics/sprite_render.cpp
        src/input/ui_button.cpp
        src/application.cpp
        src/window.cpp)

target_link_libraries(cpp_falling_sand glfw OpenGL::GL GLEW::GLEW glm::glm stb_image)
target_include_directories(cpp_falling_sand PRIVATE include app)
target_compile_definitions(cpp_falling_sand PRIVATE ROOT_DIR="${CMAKE_INSTALL_PREFIX}")

add_executable(test_util test/test_util.cpp)
target_link_libraries(test_util Catch2::Catch2 stb_image)
target_include_directories(test_util PRIVATE include)

install(TARGETS cpp_falling_sand DESTINATION bin)

install(PROGRAMS demo DESTINATION bin)
install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_PREFIX})