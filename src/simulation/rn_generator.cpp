#include <simulation/rn_generator.hpp>

rng &rng::instance() {
  static rng instance{};
  return instance;
}

bool rng::random_chance(float prob) {
  // Save a bit of work and return false if prob is zero
  if (prob == 0) { return false; }
  // Precomputed values won't work well for small probabilities
  if (prob < 0.05) { return real_(rd_) < prob; }
  // Use the precomputed values to speed things up a bit
  ++idx_;
  idx_ %= numValues_;
  return rand_float() < prob;
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

std::uint32_t rng::random_int(int max) {
  ++idx_;
  idx_ %= numValues_;
  return randomValues_[idx_] % (max + 1);
}

float rng::rand_float() {
  ++idx_;
  idx_ %= numValues_;
  return randomFloatValues_[idx_];
}