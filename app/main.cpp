// standard
#include <iostream>
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

    float vertices[] = {
            0.5f, 0.5f, 0.0f, // top right
            0.5f, -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f // top left
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    vertex_buffer vertexBuffer(sizeof(float) * 12, vertices);
    vertex_buffer_layout bufferLayout({3, GL_FLOAT, GL_FALSE});
    vertex_array vertexArray;

    vertexArray.add_buffer(vertexBuffer, bufferLayout);

    index_buffer indexBuffer(6, indices);
    indexBuffer.bind();

    const int width=100, height=100, nrChannels=3;
    char *data = new char[width*height*nrChannels];
    std::fill_n(data, width*height*nrChannels, 125);
    texture text(width, height, data);
    text.bind();
    int color = 0;

    while (!glfwWindowShouldClose(window)) {

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render container
        text.bind();
        text.update(data);
        color = (++color)%255;
        std::fill_n(data, width*height*nrChannels, color);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}
