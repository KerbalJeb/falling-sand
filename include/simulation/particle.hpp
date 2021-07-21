//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_PARTICLE_HPP
#define CPP_FALLING_SAND_PARTICLE_HPP

#include <cstdint>

using element_id_type = std::uint8_t;

struct particle_instance {
  element_id_type id{0};
  std::uint16_t age{0};
  std::int16_t vx{0};
  std::int16_t vy{0};
  std::uint8_t r{};
  std::uint8_t g{};
  std::uint8_t b{};
  std::uint8_t lastUpdated{255};
  bool is_static{false};

  void set_color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_) {
    r = r_;
    g = g_;
    b = b_;
  }
};


#endif //CPP_FALLING_SAND_PARTICLE_HPP
