//
// Created by ben on 2021-07-20.
//

#ifndef CPP_FALLING_SAND_RN_GENERATOR_HPP
#define CPP_FALLING_SAND_RN_GENERATOR_HPP

#include <random>
#include <memory>

class rng {
public:

  rng(const rng &) = delete;

  rng(rng &&) = delete;

  bool random_chance(int prob);

  float rand_float(float min, float max);

  float rand_float();

  static rng &instance();

private:
  rng() = default;

  std::random_device rd_{};
  std::uniform_real_distribution<float> real_{0, 1};
  std::mt19937 randInt_{rd_()};
};

#endif //CPP_FALLING_SAND_RN_GENERATOR_HPP
