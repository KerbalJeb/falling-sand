#include <graphics/gl/vertex_buffer.hpp>

vertex_buffer::vertex_buffer(int size, const void *data, GLenum usage) {
  glGenBuffers(1, &obj_);
  assert(obj_ != 0);
  bind();
  glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

vertex_buffer &vertex_buffer::operator=(vertex_buffer &&other) noexcept {
  if (this != &other) {
    release();
    std::swap(obj_, other.obj_);
  }
  return *this;
}
