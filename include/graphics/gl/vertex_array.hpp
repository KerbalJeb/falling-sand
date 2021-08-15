#ifndef CPP_FALLING_SAND_VERTEX_ARRAY_HPP
#define CPP_FALLING_SAND_VERTEX_ARRAY_HPP

#include <vector>
#include <initializer_list>
#include <cassert>
#include <map>

#include <GL/glew.h>

#include "vertex_buffer.hpp"

// Used to describe a single element of a vertex array
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
  // Single parameter constructor, creates a vertex buffer layout with a single attribute
  explicit vertex_buffer_layout(const vertex_attribute &attribute);

  // Multi parameter constructor, creates a vertex buffer with the given attributes
  vertex_buffer_layout(std::initializer_list<vertex_attribute> attributes);

  // Applies the vertex buffer layout to the currently bound vertex buffer and array
  void apply_layout() const;

private:
  std::vector<vertex_attribute> layout_;
  int stride_;
};


class vertex_array {
public:
  // Generate a vertex array without applying it to a buffer
  vertex_array();

  // Crate a vertex array with the given layout and apply it the the buffer
  vertex_array(const vertex_buffer &buffer, const vertex_buffer_layout &layout);

  // Not copyable
  vertex_array(const vertex_array &) = delete;

  vertex_array &operator=(const vertex_array &) = delete;

  // Move by swapping stored object id
  vertex_array(vertex_array &&other) noexcept: obj_(other.obj_) { other.obj_ = 0; }

  vertex_array &operator=(vertex_array &&other) noexcept;

  // Destroy the vertex array on destruction
  ~vertex_array() { release(); }

  // Bind the vertex array
  void bind() const { glBindVertexArray(obj_); }

  // Unbind the vertex array
  void unbind() const { glBindVertexArray(0); }

  // Apply the layout to buffer
  void add_buffer(const vertex_buffer &buffer,
                  const vertex_buffer_layout &layout) const;

private:
  GLuint obj_{0};

  void release() {
    glDeleteVertexArrays(1, &obj_);
    obj_ = 0;
  }
};

#endif //CPP_FALLING_SAND_VERTEX_ARRAY_HPP
