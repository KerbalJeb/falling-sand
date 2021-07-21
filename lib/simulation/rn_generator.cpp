#include <simulation/rn_generator.hpp>

rng &rng::instance() {
  static rng instance{};
  return instance;
}

bool rng::random_chance(int prob) {
  return (randInt_() % prob) == 0;
}

float rng::rand_float(float min, float max) {
  return min + real_(rd_) * (min - max);
}

float rng::rand_float() {
  return real_(rd_);
}

bool rng::random_chance_fast(int prob) {
  return (randomValues_[++idx_ % numValues_] % prob) == 0;
}

rng::rng() {
  randomValues_.reserve(numValues_);
  for (int i = 0; i < numValues_; ++i) {
    randomValues_.push_back(randInt_());
  }
}

std::uint32_t rng::random_int(int max) {
  return randomValues_[++idx_ % numValues_] % max;
}
