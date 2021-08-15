#include <graphics/sprite_render.hpp>

sprite_render::sprite_render(std::shared_ptr<shader_program> shader, float width, float height)
    : shader_(std::move(shader)),
      projection_(glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f)) {}

void sprite_render::draw(const std::vector<sprite> &sprites) {
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

void sprite_render::draw(const sprite &s) {
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
