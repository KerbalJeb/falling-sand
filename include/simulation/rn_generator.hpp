//
// Created by ben on 2021-07-20.
//

#ifndef CPP_FALLING_SAND_RN_GENERATOR_HPP
#define CPP_FALLING_SAND_RN_GENERATOR_HPP

#include <random>
#include <vector>

class rng {
public:

  rng(const rng &) = delete;

  rng(rng &&) = delete;

  bool random_chance(int prob);

  bool random_chance_fast(int prob);

  float rand_float(float min, float max);

  float rand_float();

  static rng &instance();

private:
  rng();

  std::random_device rd_{};
  std::uniform_real_distribution<float> real_{0, 1};
  std::mt19937 randInt_{rd_()};
  std::vector<std::uint32_t> randomValues_{};
  std::size_t idx_{};
  static constexpr std::size_t numValues_{50};
};

#endif //CPP_FALLING_SAND_RN_GENERATOR_HPP
