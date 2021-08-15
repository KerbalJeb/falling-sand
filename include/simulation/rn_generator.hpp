//
// Created by ben on 2021-07-20.
//

#ifndef CPP_FALLING_SAND_RN_GENERATOR_HPP
#define CPP_FALLING_SAND_RN_GENERATOR_HPP

#include <random>
#include <vector>

// A global random number generator
// Precaches random values to improve speed
// since there are a large number of random numbers need per frame
class rng {
public:

  rng(const rng &) = delete;

  rng(rng &&) = delete;

  // returns true p percent of the time
  // p must be between 0 and 1
  bool random_chance(float p);

  // Get a random int between min and max
  // max must be greater than min
  std::uint32_t random_int(int max, int min = 0);

  static rng &instance();

  // Get a random float between 0 and 1
  float rand_float();

private:
  rng();

  std::random_device rd_{};
  std::uniform_real_distribution<float> real_{0, 1};
  std::mt19937 randInt_{rd_()};
  std::vector<std::uint32_t> randomValues_{};
  std::vector<float> randomFloatValues_{};
  std::size_t idx_{};
  static constexpr std::size_t numValues_{250};
};

#endif //CPP_FALLING_SAND_RN_GENERATOR_HPP
