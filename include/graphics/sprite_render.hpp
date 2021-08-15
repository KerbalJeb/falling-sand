//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_SPRITE_RENDER_HPP
#define CPP_FALLING_SAND_SPRITE_RENDER_HPP

#include <utility>
#include <vector>
#include <memory>

#include <graphics/gl/shader.hpp>
#include <graphics/gl/vertex_array.hpp>
#include <graphics/gl/vertex_buffer.hpp>
#include <graphics/sprite.hpp>

#include <glm/gtc/matrix_transform.hpp>

// Utility class to render get_sprite objects
class sprite_render {
public:
  // Takes a shaded pointer to the shader program to use and the width and height of the screen in pixels
  explicit sprite_render(std::shared_ptr<shader_program> shader, float width,
                         float height);

  ~sprite_render() = default;

  // Draws all sprites in a vector
  void draw(const std::vector<sprite> &sprites);

  // Draws a single get_sprite
  void draw(const sprite &s);

private:
  std::shared_ptr<shader_program> shader_;
  const glm::mat4 projection_;
  // Unit square
  static const inline std::vector<GLfloat> vertices{
      // pos         tex
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f
  };
  static const inline vertex_buffer_layout layout_{{4, GL_FLOAT, GL_FALSE}};
  vertex_buffer buffer_{24 * sizeof(GLfloat), vertices.data()};
  vertex_array array_{buffer_, layout_};
};

#endif //CPP_FALLING_SAND_SPRITE_RENDER_HPP
