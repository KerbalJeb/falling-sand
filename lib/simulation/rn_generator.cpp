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
