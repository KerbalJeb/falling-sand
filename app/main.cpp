#include <iostream>
#include <algorithm>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <application.hpp>
#include "sand_layer.hpp"
#include "ui_layer.hpp"

const int width = 100, height = 100;

int main() {
  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }
  auto app = application::init(512, 512, "My Demo", GL_FALSE,
                               "resources/icon.png");
  sand_layer sandLayer{width, height};
  ui_layer uiLayer{};
  app->push_layer(&sandLayer);
  app->push_layer(&uiLayer);

  app->run();
  application::deinit();
  return EXIT_SUCCESS;
}
