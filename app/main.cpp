#include <iostream>
#include <algorithm>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <application.hpp>
#include "sand_layer.hpp"
#include "ui_layer.hpp"

#ifndef ROOT_DIR
#define ROOT_DIR "./"
#endif

const int width = 640, height = 320;

int main() {
  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }
  auto app = application::init(ROOT_DIR"/resources/shaders/sprite2d.vert",
                               ROOT_DIR"/resources/shaders/sprite2d.frag",
                               width * 2,
                               height * 2 + 100,
                               "My Demo",
                               GL_FALSE,
                               "resources/icon.png");
  sand_layer sandLayer{width, height, 2, application::basic_render()};
  std::vector<ui_button> buttons{
      {ROOT_DIR"/resources/sand-button.png",  10,  650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("sand");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/water-button.png", 95,  650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("water");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/wood-button.png",  180, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("wood");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/fire-button.png",  265, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("fire");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/lava-button.png",  350, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("lava");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/oil-button.png",   435, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("oil");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/wall-button.png",  520, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("wall");
        sandLayer.set_active_element(id);
      }},
      {ROOT_DIR"/resources/steam-button.png", 605, 650, 25, 75, [&sandLayer]() {
        static auto &em = element_manager::instance();
        static auto id = em.get_idx("steam");
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
