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
  random_color(std::uint8_t redMin, std::uint8_t redMax, std::uint8_t greenMin,
               std::uint8_t greenMax, std::uint8_t blueMin,
               std::uint8_t blueMax)
      : red_(redMin, redMax), green_(greenMin, greenMax),
        blue_(blueMin, blueMax) {}

  std::uint8_t red() { return red_(rd); }

  std::uint8_t green() { return green_(rd); }

  std::uint8_t blue() { return blue_(rd); }

private:
  static inline std::random_device rd{};
  std::uniform_int_distribution<std::uint8_t> red_;
  std::uniform_int_distribution<std::uint8_t> green_;
  std::uniform_int_distribution<std::uint8_t> blue_;
};

#endif //CPP_FALLING_SAND_UTIL_HPP
