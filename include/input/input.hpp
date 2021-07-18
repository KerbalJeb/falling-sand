//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_INPUT_HPP
#define CPP_FALLING_SAND_INPUT_HPP

#include <application.hpp>
#include <GLFW/glfw3.h>

class input {
public:
  int static get_mouse_button(int button) {
    return glfwGetMouseButton(
        application::instance()->get_window()->get_native(), button);
  }

  void static get_cursor(double *xPos, double *yPos) {
    glfwGetCursorPos(application::instance()->get_window()->get_native(), xPos,
                     yPos);
  }
};

#endif //CPP_FALLING_SAND_INPUT_HPP
