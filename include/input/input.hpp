//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_INPUT_HPP
#define CPP_FALLING_SAND_INPUT_HPP

#include <application.hpp>
#include <GLFW/glfw3.h>

namespace input {
  // Wrapper for glfwGetMouseButton
  int static get_mouse_button(int button) {
    return glfwGetMouseButton(application::instance()->get_window().glfw_win(), button);
  }

  // Wrapper for glfwGetCursorPos
  void static get_cursor_pos(double *xPos, double *yPos) {
    glfwGetCursorPos(application::instance()->get_window().glfw_win(), xPos, yPos);
  }
};

#endif //CPP_FALLING_SAND_INPUT_HPP
