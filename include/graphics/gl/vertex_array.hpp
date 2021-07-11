//
// Created by ben on 2021-06-29.
//

#ifndef CPP_FALLING_SAND_VERTEX_ARRAY_HPP
#define CPP_FALLING_SAND_VERTEX_ARRAY_HPP

#include <vector>
#include <initializer_list>
#include <cassert>
#include <map>

#include <GL/glew.h>

#include "vertex_buffer.hpp"

struct vertex_attribute {
    GLint count;
    GLenum type;
    GLboolean normalize;

    static inline std::map<GLenum, int> size_mapping = {
            {GL_FLOAT,        sizeof(GLfloat)},
            {GL_INT,          sizeof(GLint)},
            {GL_UNSIGNED_INT, sizeof(GLuint)},
    };
};

class vertex_buffer_layout {
public:
    vertex_buffer_layout(const vertex_attribute& attribute): layout_{}{
        layout_.push_back(attribute);
        stride_ = 0;
    }
    vertex_buffer_layout(std::initializer_list<vertex_attribute> attributes): layout_(attributes){
        stride_ = std::accumulate(layout_.begin(), layout_.end(), 0, [](int a, const vertex_attribute &b) {
            return a + vertex_attribute::size_mapping[b.type];
        });
    }

    [[nodiscard]] int get_stride() const { return stride_; }

    void apply_layout() const{
        std::size_t offset = 0;
        for (int i = 0; i < layout_.size(); ++i) {
            const auto &attrib = layout_[i];
            int element_size = vertex_attribute::size_mapping[attrib.type];
            glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalize, stride_, (void *) offset);
            glEnableVertexAttribArray(i);
            offset += element_size * attrib.count;
        }
    }

private:
    std::vector<vertex_attribute> layout_;
    int stride_;
};


class vertex_array {
public:
    vertex_array() {
        glGenVertexArrays(1, &obj_);
        assert(obj_ != 0);
    }

    vertex_array(const vertex_array &) = delete;

    vertex_array &operator=(const vertex_array &) = delete;

    vertex_array(vertex_array &&other) noexcept : obj_(other.obj_){
        other.obj_ = 0;
    }

    vertex_array &operator=(vertex_array &&other) noexcept {
        if (this != &other)
        {
            release();
            std::swap(obj_, other.obj_);
        }
        return *this;
    }

    ~vertex_array() { release(); }

    void bind() const{
        glBindVertexArray(obj_);
    }

    void unbind() const{
        glBindVertexArray(0);
    }

    void add_buffer(const vertex_buffer& buffer, const vertex_buffer_layout& layout) const{
        bind();
        buffer.bind();
        layout.apply_layout();
    }

private:
    GLuint obj_{0};

    void release() {
        glDeleteVertexArrays(1, &obj_);
        obj_ = 0;
    }
};

#endif //CPP_FALLING_SAND_VERTEX_ARRAY_HPP
