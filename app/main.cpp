#include <iostream>
#include <algorithm>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <graphics/gl/shader.hpp>
#include <graphics/gl/vertex_buffer.hpp>
#include <graphics/gl/vertex_array.hpp>
#include <graphics/gl/index_buffer.hpp>
#include <graphics/gl/texture2d.hpp>
#include <window.hpp>
#include <application.hpp>
#include <graphics/sprite_render.hpp>

const int width = 100, height = 100, nrChannels = 3;
const int data_size = width * height * nrChannels;
std::uint8_t *data = new std::uint8_t[data_size];

int main() {
  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }
  auto app = application::init(512, 512, "My Demo", GL_FALSE);

  auto program = new shader_program{"shaders/sprite2d.vert",
                                    "shaders/sprite2d.frag"};


  std::fill_n(data, width * height * nrChannels, 0);
  texture2d text(width, height, data);

  std::vector<sprite> scene;
  scene.push_back({&text, glm::vec2(0.0f), glm::vec2(width, height)});

  sprite_render render{program, width, height};
  app->run();
  application::deinit();
  return EXIT_SUCCESS;
}
