//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_SPRITE_H
#define CPP_FALLING_SAND_SPRITE_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <graphics/gl/texture2d.hpp>

// A basic get_sprite object
struct sprite {
  texture2d *texture; // Image texture of the get_sprite
  glm::vec2 pos; // The position of the get_sprite (in pixels)
  glm::vec2 scale; // The scale of the get_sprite
  glm::vec3 color = glm::vec3{1.0f}; // The color tint of the get_sprite
};

#endif //CPP_FALLING_SAND_SPRITE_H
