//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_UTIL_HPP
#define CPP_FALLING_SAND_UTIL_HPP

#include <random>
#include <simulation/simulation_canvas.hpp>

constexpr std::int16_t gravity_accel = 2;

template<class UnaryPredicate>
particle_instance &
find_in_line(simulation_canvas &canvas, int x0, int x1, int y0, int y1,
             UnaryPredicate p) {
  assert(x0 == x1);
  assert(y1 >= y0);
  auto pl = &canvas.get_particle(x0, y0);
  for (int y = y0 + 1; y <= y1; ++y) {
    if (!canvas.in_canvas(x0, y)) { return *pl; }
    auto &pi = canvas.get_particle(x0, y);
    if (!p(pi)) {
      return *pl;
    }
    pl = &pi;
  }
  return *pl;
}

class random_color {
public:
  random_color(float minScale, float maxScale) : dist_(minScale, maxScale) {}

  void set_color(std::uint8_t &r, std::uint8_t &g, std::uint8_t &b) {
    float scale = dist_(rd);
    r = static_cast<std::uint8_t>(std::clamp(scale * r, 0.0f, 255.0f));
    g = static_cast<std::uint8_t>(std::clamp(scale * g, 0.0f, 255.0f));
    b = static_cast<std::uint8_t>(std::clamp(scale * b, 0.0f, 255.0f));
  }

private:
  static inline std::random_device rd{};
  std::uniform_real_distribution<float> dist_;
};

#endif //CPP_FALLING_SAND_UTIL_HPP
