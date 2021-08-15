//
// Created by ben on 2021-07-10.
//

#ifndef CPP_FALLING_SAND_TEXTURE2D_HPP
#define CPP_FALLING_SAND_TEXTURE2D_HPP

#include <cstddef>
#include <iostream>
#include <GL/glew.h>

// A wrapper class for a 2d opengl texture
class texture2d {
public:
  // Creates a opengl texture of size width by height using the provided pixel data
  // The memory block pointed to by pixel_data large enough to contain all pixel data (width*height*pixelSize)
  // minMagFilter is the filter type to use when magnifying or minifying the texture, defaults to neares
  // format is the format of the image, defaults to RGB
  texture2d(std::size_t width, std::size_t height, const void *pixel_data,
            GLenum format = GL_RGB, GLint minMagFilter = GL_NEAREST);

  // The texture is a GPU object, so it cannot easily be copied
  texture2d(const texture2d &) = delete;

  texture2d &operator=(const texture2d &) = delete;

  // A texture is moved by swapping the internal object ids
  texture2d(texture2d &&other) noexcept;

  texture2d &operator=(texture2d &&other);

  // Delete the opengl texture on destruction
  ~texture2d() { release(); }

  // Update the texture with new data
  // new_data must point to a region of memory at least a big as the texture size (width*height*pixelSize)
  void update(const void *new_data);

  // Bind the texture
  void bind() { glBindTexture(GL_TEXTURE_2D, obj_); }

  // Unbind the texture
  void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

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
