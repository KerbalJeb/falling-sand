//
// Created by ben on 2021-06-28.
//

#ifndef CPP_FALLING_SAND_WINDOW_HPP
#define CPP_FALLING_SAND_WINDOW_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ra::graphics{
    GLFWwindow *makeWindow(int width, int height, const std::string &title) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        return window;
    }
}

#endif //CPP_FALLING_SAND_WINDOW_HPP
