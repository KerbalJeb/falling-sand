// Created by ben on 2021-06-28.
//

#ifndef CPP_FALLING_SAND_WINDOW_HPP
#define CPP_FALLING_SAND_WINDOW_HPP

#include <events/application_events.hpp>
#include <events/mouse_events.hpp>
#include <events/key_events.hpp>
#include <events/drop_event.hpp>
#include <graphics/image.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <filesystem>
#include <map>
#include <vector>
#include <cassert>

// A wrapper class for GLFW windows
class window {
public:
  using event_handler_func = std::function<void(event &)>;

  // Create window of size width x height pixels
  // Window can be resized if resizable is true
  // The event handler function is call everytime a window event is generate (ex. input)
  window(int width, int height, const std::string &title, GLboolean resizable,
         const event_handler_func &handler);

  // Generate a window with a custom icon, loaded from iconPath
  // iconPath must be the path of a valid image
  window(int width, int height, const std::string &title, GLboolean resizable,
         const std::filesystem::path &iconPath,
         const event_handler_func &handler);


  ~window() { shut_down(); }

  void shut_down() { glfwDestroyWindow(window_); }

  // Called every frame
  void update();

  // Set the cursor to a custom image
  void set_cursor(GLFWimage *img);

  void use_custom_cursor();

  void use_standard_cursor();

  // Get a pointer to the GLFWwindow object
  GLFWwindow *glfw_win() { return window_; }

  // The window width (px)
  [[nodiscard]] std::size_t width() const { return width_; }

  // The window height (px)
  [[nodiscard]] std::size_t height() const { return height_; }

private:
  struct window_data {
    event_handler_func event_handler;
  };
  GLFWwindow *window_;
  std::string title_;
  std::size_t width_, height_;
  window_data data_;
  GLFWcursor *customCursor_{nullptr};
  GLFWcursor *standardCursor_{glfwCreateStandardCursor(GLFW_HAND_CURSOR)};
};


#endif //CPP_FALLING_SAND_WINDOW_HPP
