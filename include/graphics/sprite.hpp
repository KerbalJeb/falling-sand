//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_SPRITE_H
#define CPP_FALLING_SAND_SPRITE_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <graphics/gl/texture2d.hpp>

struct sprite {
  texture2d *texture;
  glm::vec2 pos;
  glm::vec2 scale;
  glm::vec3 color = glm::vec3{1.0f};
};

#endif //CPP_FALLING_SAND_SPRITE_H
