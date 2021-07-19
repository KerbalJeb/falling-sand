//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_SAND_HPP
#define CPP_FALLING_SAND_SAND_HPP

#include <simulation/elements_common.hpp>

namespace elements {
  class create_sand {
  public:
    particle_instance operator()() {
      float colorScale = color(rd);
      return particle_instance{1, 0, 0, 0,
                               static_cast<uint8_t>(194 * colorScale),
                               static_cast<uint8_t>(178 * colorScale),
                               static_cast<uint8_t>(128 * colorScale)};
    }

  private:
    static inline std::random_device rd{};
    std::uniform_real_distribution<float> color{0.8, 1};
  };

  class update_sand {
  public:
    void operator()(simulation_canvas &canvas, int x, int y) {
      auto &e = canvas.get_particle(x, y);

      if (canvas.in_canvas(x, y + 1)) {
        auto &below = canvas.get_particle(x, y + 1);
        if (below.id == element::empty_id()) {
          std::swap(below, e);
          return;
        }
      }

      if (canvas.in_canvas(x - 1, y + 1)) {
        auto &below_left = canvas.get_particle(x - 1, y + 1);
        if (below_left.id == element::empty_id()) {
          std::swap(below_left, e);
          return;
        }
      }

      if (canvas.in_canvas(x + 1, y + 1)) {
        auto &below_right = canvas.get_particle(x + 1, y + 1);
        if (below_right.id == element::empty_id()) {
          std::swap(below_right, e);
          return;
        }
      }
    }
  };
}
#endif //CPP_FALLING_SAND_SAND_HPP
