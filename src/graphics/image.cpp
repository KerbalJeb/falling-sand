#include <graphics/image.hpp>

image::image(const std::filesystem::path &path) {
  img_ = stbi_load(path.c_str(), &width_, &height_, &channels_,
                   STBI_rgb_alpha);
  assert(img_ != nullptr);
}

image::image(int width, int height, int channels) : width_(width), height_(height),
                                                    channels_(channels) {
  // use malloc since stbi does to simplify freeing of memory
  img_ = static_cast<uint8_t *>(std::malloc(width * height * channels));
  std::fill(img_, img_ + size(), 0);
}

image::~image() {
  // stbi uses malloc, so we need to use free instead of delete
  std::free(img_);
}
