//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_IMAGE_LOADER_HPP
#define CPP_FALLING_SAND_IMAGE_LOADER_HPP

#include <filesystem>
#include <cassert>
#include <stb_image/stb_image.h>

class image_loader {
public:
  explicit image_loader(const std::filesystem::path &path) {
    img = stbi_load(path.c_str(), &width_, &height_, &channels_,
                    STBI_rgb_alpha);
    assert(img != nullptr);
  }

  ~image_loader() {
    free(img);
  }

  std::uint8_t *img_ptr() { return img; }

  [[nodiscard]] const std::uint8_t *img_ptr() const { return img; }

  [[nodiscard]] int width() const { return width_; }

  [[nodiscard]] int height() const { return height_; }

  [[nodiscard]] int channels() const { return channels_; }

private:
  int width_{}, height_{}, channels_{};
  std::uint8_t *img{nullptr};
};

#endif //CPP_FALLING_SAND_IMAGE_LOADER_HPP
