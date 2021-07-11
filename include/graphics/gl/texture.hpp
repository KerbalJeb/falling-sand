//
// Created by ben on 2021-07-10.
//

#ifndef CPP_FALLING_SAND_TEXTURE_HPP
#define CPP_FALLING_SAND_TEXTURE_HPP

#include <cstddef>
#include <GL/glew.h>

class texture {
public:
    texture(std::size_t width, std::size_t height, const void *pixel_data) : width_(width), height_(height) {
        glGenTextures(1, &obj_);
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
    }

    texture(const texture &) = delete;

    texture &operator=(const texture &) = delete;

    texture(texture &&other) : obj_(other.obj_), width_(other.width_), height_(other.height_) {
        other.obj_ = 0;
        other.height_ = 0;
        other.width_ = 0;
    }

    texture &operator=(texture &&other){
        if (this != &other){
            release();
            std::swap(obj_, other.obj_);
            std::swap(height_, other.height_);
            std::swap(width_, other.width_);
        }
        return *this;
    }

    ~texture() { release(); }

    void update(char *new_data) {
        glTexSubImage2D(obj_, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, new_data);
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, obj_);
    }

    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    std::size_t width_;
    std::size_t height_;
    GLuint obj_{0};

    void release() {
        glDeleteTextures(1, &obj_);
        obj_ = 0;
    }
};

#endif //CPP_FALLING_SAND_TEXTURE_HPP
