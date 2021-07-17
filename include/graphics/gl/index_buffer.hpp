//
// Created by ben on 2021-06-29.
//

#ifndef CPP_FALLING_SAND_INDEX_BUFFER_HPP
#define CPP_FALLING_SAND_INDEX_BUFFER_HPP

#include <GL/glew.h>

class index_buffer {
public:
    index_buffer(int count, const void *data, GLenum method = GL_STATIC_DRAW) {
        glGenBuffers(1, &obj_);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, method);
    }

    index_buffer(const index_buffer &) = delete;

    index_buffer &operator=(const index_buffer &) = delete;

    index_buffer(index_buffer &&other): obj_(other.obj_){
        other.obj_ = 0;
    }

    index_buffer &operator=(index_buffer &&other){
        release();
        std::swap(obj_, other.obj_);
        return *this;
    }

    ~index_buffer() { release(); }

    void bind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_);
    }

    void unbind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

private:
    GLuint obj_{0};

    void release() {
        glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &obj_);
        obj_ = 0;
    }
};

#endif //CPP_FALLING_SAND_INDEX_BUFFER_HPP
