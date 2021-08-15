#include <iostream>
#include <algorithm>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <application.hpp>
#include "sand_layer.hpp"
#include "ui_layer.hpp"

// This should be set by cmake to the installation directory
#ifndef ROOT_DIR
static_assert(false, "ROOT_DIR must be defined");
#endif

// The width and high of the simulation area
const int width = 640, height = 320;

int main() {
  auto app = application::init(ROOT_DIR"/resources/shaders/sprite2d.vert",
                               ROOT_DIR"/resources/shaders/sprite2d.frag",
                               width * 2,
                               height * 2 + 100,
                               "Falling Sand",
                               GL_FALSE,
                               ROOT_DIR"/resources/icon.png");
  sand_layer sandLayer{width, height, 2, application::basic_render()};

  // Generates the callbacks for the element selection buttons
  auto gen_button_action = [&sandLayer](const std::string &name) {
    auto id = sandLayer.get_canvas().get_element_manager().find_id(name);
    return [&sandLayer, id]() {
      sandLayer.set_active_element(id);
    };
  };

  std::vector<ui_button> buttons{
      {ROOT_DIR"/resources/sand-button.png",  10,  650, 25, 75, gen_button_action("sand")},
      {ROOT_DIR"/resources/water-button.png", 95,  650, 25, 75, gen_button_action("water")},
      {ROOT_DIR"/resources/wood-button.png",  180, 650, 25, 75, gen_button_action("wood")},
      {ROOT_DIR"/resources/fire-button.png",  265, 650, 25, 75, gen_button_action("fire")},
      {ROOT_DIR"/resources/lava-button.png",  350, 650, 25, 75, gen_button_action("lava")},
      {ROOT_DIR"/resources/oil-button.png",   435, 650, 25, 75, gen_button_action("oil")},
      {ROOT_DIR"/resources/wall-button.png",  520, 650, 25, 75, gen_button_action("wall")},
      {ROOT_DIR"/resources/steam-button.png", 605, 650, 25, 75, gen_button_action("steam")},
      {ROOT_DIR"/resources/salt-button.png",  690, 650, 25, 75, gen_button_action("salt")},
  };

  ui_layer uiLayer{application::basic_render(), buttons};

  // UI layer is on top so it get priority on input events
  app->push_layer(&uiLayer);
  app->push_layer(&sandLayer);

  app->run();
  application::deinit();
  return EXIT_SUCCESS;
}
