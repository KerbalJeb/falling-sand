// standard
#include <iostream>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <graphics/gl/shader.hpp>
#include <graphics/gl/vertex_buffer.hpp>
#include <graphics/gl/vertex_array.hpp>
#include <graphics/window.hpp>
#include <graphics/gl/vertex_array.hpp>
GLuint vao = 0;
GLuint EBO = 0;


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

    shader_program program{"shaders/basic.vert", "shaders/basic.frag"};
    program.bind();
    program.set_uniform4f("color", 1, 0, 0, 1);

    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    vertex_buffer vertexBuffer(vertices, sizeof(float)*9);
    vertex_buffer_layout bufferLayout({3, GL_FLOAT, GL_FALSE});
    vertex_array vertexArray;

    vertexArray.add_buffer(vertexBuffer, bufferLayout);






    while (!glfwWindowShouldClose(window))
    {

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render container
        program.bind();
        vertexArray.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}
