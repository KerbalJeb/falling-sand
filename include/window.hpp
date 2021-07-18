// Created by ben on 2021-06-28.
//

#ifndef CPP_FALLING_SAND_WINDOW_HPP
#define CPP_FALLING_SAND_WINDOW_HPP

#include <events/application_event.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>
#include <vector>

class window {
public:
  using event_handler_func = std::function<void(event &)>;

  window(int width, int height, const std::string &title, GLboolean resizable,
         const event_handler_func &handler)
      : title_(title),
        width_(width),
        height_(height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window_, &data_);
    glfwMakeContextCurrent(window_);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      glfwTerminate();
      assert(false);
    }

    set_handler(handler);

    glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
      auto data = static_cast<window_data *>(glfwGetWindowUserPointer(window));
      window_close_event e;
      data->event_handler(e);
    });

  }

  void shut_down() { glfwDestroyWindow(window_); }

  void update() {
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  void set_handler(const event_handler_func &f) { data_.event_handler = f; }

  GLFWwindow *get_native() { return window_; }

private:
  struct window_data {
    event_handler_func event_handler;
  };
  GLFWwindow *window_;
  std::string title_;
  std::size_t width_, height_;
  window_data data_;
};


#endif //CPP_FALLING_SAND_WINDOW_HPP
