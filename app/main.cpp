// standard
#include <iostream>
// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// local
#include <helper/shader.hpp>
#include <helper/window.hpp>

GLuint vao = 0;
GLuint EBO = 0;

void makeVao(ra::graphics::shader_program program, GLuint &vao, GLuint &vbo) {
    static const GLfloat vertices[] = {
            1.0f, 1.0f, 0.0f, // top right
            1.0f, -1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f // top left
    };
    static const GLuint indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GLuint aPosition = program.get_attrib_location("aPosition");
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(aPosition);
}

void refresh(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
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

    ra::graphics::shader_program program{"shaders/basic.vert", "shaders/basic.frag"};
    program.use();
    program.set_uniform4f("color", 1, 0, 0, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    GLuint vbo;
    makeVao(program, vao, vbo);
    glfwSetWindowRefreshCallback(window, refresh);
    while (!glfwWindowShouldClose(window)) { glfwWaitEvents(); }

    return EXIT_SUCCESS;
}
