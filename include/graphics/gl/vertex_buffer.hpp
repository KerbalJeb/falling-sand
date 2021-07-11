//
// Created by ben on 2021-07-11.
//

#ifndef CPP_FALLING_SAND_VERTEX_BUFFER_HPP
#define CPP_FALLING_SAND_VERTEX_BUFFER_HPP
#include <vector>
#include <map>

#include <GL/glew.h>

struct vertex_attribute {
    GLuint count;
    GLenum type;
    GLboolean normalize;

    static inline std::map<GLenum, GLuint> size_mapping = {
            {GL_FLOAT, sizeof(GLfloat)},
            {GL_INT, sizeof(GLint)},
            {GL_UNSIGNED_INT, sizeof(GLuint)},
    };
};

class vertex_buffer {
public:
    vertex_buffer(const std::vector<vertex_attribute>& layout);

    vertex_buffer(const vertex_buffer &) = delete;

    vertex_buffer &operator=(const vertex_buffer &) = delete;

    vertex_buffer(vertex_buffer &&other);

    vertex_buffer &operator=(vertex_buffer &&other);

    ~vertex_buffer();

    void bind();

private:
    GLuint obj_{0};
    std::size_t stride_{0};
    std::vector<vertex_attribute> layout_;

    void release();
};

#endif //CPP_FALLING_SAND_VERTEX_BUFFER_HPP
