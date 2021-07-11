//
// Created by ben on 2021-07-11.
//

#ifndef CPP_FALLING_SAND_VERTEX_BUFFER_HPP
#define CPP_FALLING_SAND_VERTEX_BUFFER_HPP

#include <utility>
#include <numeric>

#include <GL/glew.h>
#include <cassert>

class vertex_buffer {
public:
    explicit vertex_buffer(const void* data, int size, GLenum usage=GL_STATIC_DRAW) {
        glGenBuffers(1, &obj_);
        assert(obj_ != 0);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    vertex_buffer(const vertex_buffer &) = delete;

    vertex_buffer &operator=(const vertex_buffer &) = delete;

    vertex_buffer(vertex_buffer &&other) noexcept : obj_(other.obj_){
        other.obj_ = 0;
    }

    vertex_buffer &operator=(vertex_buffer &&other) noexcept {
        if (this != &other){
            release();
            std::swap(obj_, other.obj_);
        }
        return *this;
    }

    ~vertex_buffer() { release(); }

    void bind() const{
        glBindBuffer(GL_ARRAY_BUFFER, obj_);
    }

    void unbind() const{
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    GLuint obj_{0};

    void release() {
        glDeleteBuffers(1, &obj_);
        obj_ = 0;
    }
};

#endif //CPP_FALLING_SAND_VERTEX_BUFFER_HPP
