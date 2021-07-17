//
// Created by ben on 2021-07-10.
//

#ifndef CPP_FALLING_SAND_TEXTURE2D_HPP
#define CPP_FALLING_SAND_TEXTURE2D_HPP

#include <cstddef>
#include <iostream>
#include <GL/glew.h>

class texture2d {
public:
  texture2d(std::size_t width, std::size_t height, const void *pixel_data)
      : width_(width), height_(height) {
    glGenTextures(1, &obj_);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pixel_data);
  }

  texture2d(const texture2d &) = delete;

  texture2d &operator=(const texture2d &) = delete;

  texture2d(texture2d &&other) : obj_(other.obj_), width_(other.width_),
                                 height_(other.height_) {
    other.obj_ = 0;
    other.height_ = 0;
    other.width_ = 0;
  }

  texture2d &operator=(texture2d &&other) {
    if (this != &other) {
      release();
      std::swap(obj_, other.obj_);
      std::swap(height_, other.height_);
      std::swap(width_, other.width_);
    }
    return *this;
  }

  ~texture2d() { release(); }

    void update(const void *new_data) {
        bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, new_data);
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

#endif //CPP_FALLING_SAND_TEXTURE2D_HPP
