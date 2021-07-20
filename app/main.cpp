#include <iostream>
#include <algorithm>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <application.hpp>
#include "sand_layer.hpp"
#include "ui_layer.hpp"

const int width = 640, height = 320;

int main() {
  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }
  auto app = application::init(width * 2, height * 2 + 100, "My Demo", GL_FALSE,
                               "resources/icon.png");
  sand_layer sandLayer{width, height, 2, application::basic_render()};
  std::vector<ui_button> buttons{
      {"resources/sand-button.png",  10, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("sand");
        sandLayer.set_active_element(id);
      }},
      {"resources/water-button.png", 95, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("water");
        sandLayer.set_active_element(id);
      }},
  };

  ui_layer uiLayer{application::basic_render(), buttons};


  app->push_layer(&sandLayer);
  app->push_layer(&uiLayer);

  app->run();
  application::deinit();
  return EXIT_SUCCESS;
}
