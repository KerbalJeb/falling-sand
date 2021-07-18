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

class sprite_render {
public:
  explicit sprite_render(std::shared_ptr<shader_program> shader, float width,
                         float height)
      : shader_(std::move(shader)),
        projection_(glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f)) {}

  ~sprite_render() = default;

  void draw(const std::vector<sprite> &sprites) {
    shader_->bind();
    shader_->set_uniform_mat4("projection", projection_);
    for (const auto &s: sprites) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(s.pos, 0.0f));
      model = glm::scale(model, glm::vec3(s.scale, 1.0f));

      shader_->set_uniform_mat4("model", model);
      shader_->set_uniform3f("spriteColor", s.color);
      s.texture->bind();
      array_.bind();
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  void draw(const sprite &s) {
    shader_->bind();
    shader_->set_uniform_mat4("projection", projection_);
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(s.pos, 0.0f));
    model = glm::scale(model, glm::vec3(s.scale, 1.0f));

    shader_->set_uniform_mat4("model", model);
    shader_->set_uniform3f("spriteColor", s.color);
    s.texture->bind();
    array_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

private:
  std::shared_ptr<shader_program> shader_;
  const glm::mat4 projection_;
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
