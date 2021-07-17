// Created by ben on 2021-06-28.
//

#ifndef CPP_FALLING_SAND_WINDOW_HPP
#define CPP_FALLING_SAND_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>
#include <vector>

class window {
public:
  window(int width, int height, const std::string &title, GLboolean resizable)
      : title_(title),
        width_(width),
        height_(height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(window_);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      glfwTerminate();
      assert(false);
    }
  }

  void shut_down() { glfwDestroyWindow(window_); }

  void update() {
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  GLFWwindow *get_native() { return window_; }

private:
  GLFWwindow *window_;
  std::string title_;
  std::size_t width_, height_;
};


#endif //CPP_FALLING_SAND_WINDOW_HPP
