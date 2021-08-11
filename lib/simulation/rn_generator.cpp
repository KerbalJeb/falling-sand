#include <simulation/rn_generator.hpp>

rng &rng::instance() {
  static rng instance{};
  return instance;
}

bool rng::random_chance(float prob) {
  ++idx_;
  idx_ %= numValues_;
  return rand_float() < (prob + minRealValue_);
}

rng::rng() {
  randomValues_.reserve(numValues_);
  randomFloatValues_.reserve(numValues_);

  for (int i = 0; i < numValues_; ++i) {
    randomValues_.push_back(randInt_());
    randomFloatValues_.push_back(real_(rd_));
  }
  minRealValue_ = *std::min_element(randomFloatValues_.begin(), randomFloatValues_.end());
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
