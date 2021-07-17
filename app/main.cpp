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
#include <graphics/sprite_render.hpp>

const int width = 100, height = 100, nrChannels = 3;
const int data_size = width * height * nrChannels;
std::uint8_t *data = new std::uint8_t[data_size];

int main() {
  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }
  GLFWwindow *window = ra::graphics::makeWindow(512, 512, "My Demo");

    glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  shader_program program{"shaders/sprite2d.vert", "shaders/sprite2d.frag"};


  std::fill_n(data, width * height * nrChannels, 0);
  texture2d text(width, height, data);

  std::vector<sprite> scene;
  scene.push_back({&text, glm::vec2(0.0f), glm::vec2(width, height)});

  sprite_render render{&program, width, height};

  while (!glfwWindowShouldClose(window)) {

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      double x = (xpos / 512);
      double y = (ypos / 512);

      int xcord = static_cast<int>(x * width);
      int ycord = static_cast<int>(y * height);
      xcord = std::clamp(xcord, 0, width);
      ycord = std::clamp(ycord, 0, height);
      int idx = (ycord * width + xcord) * 3;
      std::fill_n(&data[idx + 1], 2, 255);
    }

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render container
    text.update(data);
    render.draw(scene);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}
