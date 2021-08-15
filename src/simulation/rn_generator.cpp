#include <simulation/rn_generator.hpp>

rng &rng::instance() {
  static rng instance{};
  return instance;
}

bool rng::random_chance(float p) {
  // Save a bit of work and return false if p is zero
  if (p == 0) { return false; }
  // Precomputed values won't work well for small probabilities
  if (p < 5.0f / numValues_) { return real_(rd_) < p; }
  // Use the precomputed values to speed things up a bit
  ++idx_;
  idx_ %= numValues_;
  return rand_float() < p;
}

rng::rng() {
  randomValues_.reserve(numValues_);
  randomFloatValues_.reserve(numValues_);

  for (int i = 0; i < numValues_; ++i) {
    randomValues_.push_back(randInt_());
    randomFloatValues_.push_back(real_(rd_));
  }
  *std::min_element(randomFloatValues_.begin(), randomFloatValues_.end());
}

std::uint32_t rng::random_int(int max, int min) {
  ++idx_;
  idx_ %= numValues_;
  max -= min;
  ++max;
  return min + randomValues_[idx_] % max;
}

float rng::rand_float() {
  ++idx_;
  idx_ %= numValues_;
  return randomFloatValues_[idx_];
}
