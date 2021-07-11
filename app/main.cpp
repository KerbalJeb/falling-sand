// standard
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
#include <graphics/gl/texture.hpp>
#include <graphics/window.hpp>

const int width = 100, height = 100, nrChannels = 3;
const int data_size = width * height * nrChannels;
std::uint8_t *data = new std::uint8_t[data_size];

void on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double x = (xpos / 512);
        double y = ((512 - ypos) / 512);

        int xcord = static_cast<int>(x * width);
        int ycord = static_cast<int>(y * height);
        xcord = std::clamp(xcord, 0, width);
        ycord = std::clamp(ycord, 0, height);
        int idx = (ycord * width + xcord) * 3;
        std::fill_n(&data[idx + 1], 2, 255);
    }
}

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

    shader_program program{"shaders/tex.vert", "shaders/tex.frag"};
    program.bind();
    program.set_uniform4f("color", 1, 0, 0, 1);

    std::vector vertices{
            +1.0f, +1.0f, 0.0f, +1.0f, +1.0f, // top right
            +1.0f, -1.0f, 0.0f, +1.0f, +0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, +0.0f, +0.0f, // bottom left
            -1.0f, +1.0f, 0.0f, +0.0f, +1.0f, // top left
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    vertex_buffer vertexBuffer(sizeof(float) * 20, vertices.data());
    vertex_buffer_layout bufferLayout({{3, GL_FLOAT, GL_FALSE},
                                       {2, GL_FLOAT, GL_FALSE}});
    vertex_array vertexArray;

    vertexArray.add_buffer(vertexBuffer, bufferLayout);

    index_buffer indexBuffer(6, indices);
    indexBuffer.bind();

    std::fill_n(data, width * height * nrChannels, 0);
    texture text(width, height, data);
    text.bind();

    glfwSetCursorPosCallback(window, on_mouse_move);
    while (!glfwWindowShouldClose(window)) {

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render container
        text.update(data);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}
