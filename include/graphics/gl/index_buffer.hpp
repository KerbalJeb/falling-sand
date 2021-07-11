//
// Created by ben on 2021-06-29.
//

#ifndef CPP_FALLING_SAND_INDEX_BUFFER_HPP
#define CPP_FALLING_SAND_INDEX_BUFFER_HPP

#include <GL/glew.h>

class index_buffer {
public:
    index_buffer();

    index_buffer(const index_buffer &) = delete;

    index_buffer &operator=(const index_buffer &) = delete;

    index_buffer(index_buffer &&other);

    index_buffer &operator=(index_buffer &&other);

    void bind();

private:
    void release();

    GLuint obj_{0};
};

#endif //CPP_FALLING_SAND_INDEX_BUFFER_HPP
