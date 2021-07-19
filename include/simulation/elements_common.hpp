//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_ELEMENTS_COMMON_HPP
#define CPP_FALLING_SAND_ELEMENTS_COMMON_HPP

#include <random>
#include <simulation/simulation_canvas.hpp>

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

#endif //CPP_FALLING_SAND_ELEMENTS_COMMON_HPP
