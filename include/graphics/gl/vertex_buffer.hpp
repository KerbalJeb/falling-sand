#ifndef CPP_FALLING_SAND_VERTEX_BUFFER_HPP
#define CPP_FALLING_SAND_VERTEX_BUFFER_HPP

#include <utility>
#include <numeric>

#include <GL/glew.h>
#include <cassert>

// Wrapper call for opengl vertex buffers
class vertex_buffer {
public:
  // Create a vertex buffer of the given size using the given data
  // data must point to a region of memory of at least size bytes
  explicit vertex_buffer(int size, const void *data, GLenum usage = GL_STATIC_DRAW);

  // Not copyable
  vertex_buffer(const vertex_buffer &) = delete;

  vertex_buffer &operator=(const vertex_buffer &) = delete;

  // Move by swapping object ids
  vertex_buffer(vertex_buffer &&other) noexcept: obj_(other.obj_) { other.obj_ = 0; }

  vertex_buffer &operator=(vertex_buffer &&other) noexcept;

  // Destroy the vertex buffer on destruction
  ~vertex_buffer() { release(); }

  // Binds the vertex buffer
  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, obj_); }

  // Unbinds the vertex buffer
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
  GLuint obj_{0};

  void release() {
    glDeleteBuffers(1, &obj_);
    obj_ = 0;
  }
};

#endif //CPP_FALLING_SAND_VERTEX_BUFFER_HPP
