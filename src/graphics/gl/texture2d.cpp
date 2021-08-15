#include <graphics/gl/texture2d.hpp>

texture2d::texture2d(std::size_t width, std::size_t height, const void *pixel_data, GLenum format, GLint minMagFilter)
    : width_(width), height_(height) {
  glGenTextures(1, &obj_);
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
               GL_UNSIGNED_BYTE, pixel_data);
}

texture2d::texture2d(texture2d &&other) noexcept: obj_(other.obj_), width_(other.width_),
                                                  height_(other.height_) {
  other.obj_ = 0;
  other.height_ = 0;
  other.width_ = 0;
}

texture2d &texture2d::operator=(texture2d &&other) {
  if (this != &other) {
    release();
    std::swap(obj_, other.obj_);
    std::swap(height_, other.height_);
    std::swap(width_, other.width_);
  }
  return *this;
}

void texture2d::update(const void *new_data) {
  bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, new_data);
}
