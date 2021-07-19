//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_EMPTY_HPP
#define CPP_FALLING_SAND_EMPTY_HPP

#include <simulation/element.hpp>
#include <simulation/particle.hpp>

namespace elements {
  class create_empty {
  public:
    particle_instance operator()() {
      return particle_instance{};
    }
  };
}

#endif //CPP_FALLING_SAND_EMPTY_HPP
