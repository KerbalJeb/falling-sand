//
// Created by ben on 2021-06-29.
//

#ifndef CPP_FALLING_SAND_VERTEX_ARRAY_HPP
#define CPP_FALLING_SAND_VERTEX_ARRAY_HPP

#include <vector>
#include <cassert>

#include <GL/glew.h>

class vertex_array {
public:
    vertex_array();

    vertex_array(const vertex_array &) = delete;

    vertex_array &operator=(const vertex_array &) = delete;

    vertex_array(vertex_array &&other);

    vertex_array &operator=(vertex_array &&other);

    ~vertex_array();

    void bind();

private:
    GLuint obj_{0};

    void release();
};

#endif //CPP_FALLING_SAND_VERTEX_ARRAY_HPP
