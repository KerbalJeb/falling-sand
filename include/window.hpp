// Created by ben on 2021-06-28.
//

#ifndef CPP_FALLING_SAND_WINDOW_HPP
#define CPP_FALLING_SAND_WINDOW_HPP

#include <events/application_event.hpp>
#include <events/mouse_events.hpp>
#include <events/key_events.hpp>
#include <graphics/image.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <filesystem>
#include <map>
#include <vector>
#include <cassert>

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

    glfwSetMouseButtonCallback(window_, [](auto w, int b, int a, int m) {
      auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
      if (a == GLFW_PRESS) {
        mouse_press_event e{b, m};
        data->event_handler(e);
      }
    });

    glfwSetKeyCallback(window_, [](auto w, int b, int, int a, int mods) {
      auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
      if (a == GLFW_PRESS) {
        key_down_event e{b, mods};
        data->event_handler(e);
      }
    });

    glfwSetCursorPosCallback(window_, [](auto w, double x, double y) {
      auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
      mouse_moves_event e{static_cast<int>(x), static_cast<int>(y)};
      data->event_handler(e);
    });
  }

  window(int width, int height, const std::string &title, GLboolean resizable,
         const std::filesystem::path &iconPath,
         const event_handler_func &handler)
      : window(width, height, title, resizable, handler) {
    image icon{iconPath};
    GLFWimage iconImg = icon.get_glfw_image();
    glfwSetWindowIcon(window_, 1, &iconImg);
  }


  ~window() { shut_down(); }

  void shut_down() { glfwDestroyWindow(window_); }

  void update() {
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  void set_handler(const event_handler_func &f) { data_.event_handler = f; }

  void set_cursor(GLFWimage *img) {
    auto newCursor = glfwCreateCursor(img, img->width / 2, img->height / 2);
    glfwSetCursor(window_, newCursor);
    if (activeCursor_ != nullptr) {
      glfwDestroyCursor(activeCursor_);
    }
    activeCursor_ = newCursor;
  }

  GLFWwindow *get_native() { return window_; }

  [[nodiscard]] std::size_t width() const { return width_; }

  [[nodiscard]] std::size_t height() const { return height_; }

private:
  struct window_data {
    event_handler_func event_handler;
  };
  GLFWwindow *window_;
  std::string title_;
  std::size_t width_, height_;
  window_data data_;
  GLFWcursor *activeCursor_{nullptr};
};


#endif //CPP_FALLING_SAND_WINDOW_HPP
