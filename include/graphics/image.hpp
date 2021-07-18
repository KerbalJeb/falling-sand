//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_IMAGE_HPP
#define CPP_FALLING_SAND_IMAGE_HPP

#include <filesystem>
#include <cassert>
#include <stb_image/stb_image.h>
#include <graphics/gl/texture2d.hpp>

class image {
public:
  explicit image(const std::filesystem::path &path) {
    img_ = stbi_load(path.c_str(), &width_, &height_, &channels_,
                     STBI_rgb_alpha);
    assert(img_ != nullptr);
    switch (channels_) {
      case 3:
        img_format_ = GL_RGB;
        break;
      case 4:
        img_format_ = GL_RGBA;
        break;
      default:
        assert(false);
    }
  }

  ~image() {
    free(img_);
  }

  std::uint8_t *img_ptr() { return img_; }

  [[nodiscard]] const std::uint8_t *img_ptr() const { return img_; }

  [[nodiscard]] int width() const { return width_; }

  [[nodiscard]] int height() const { return height_; }

  [[nodiscard]] int channels() const { return channels_; }

  texture2d create_texture() {
    return texture2d(width_, height_, img_, img_format_, GL_LINEAR);
  }

  std::unique_ptr<texture2d> create_unique_texture() {
    return std::make_unique<texture2d>(width_, height_, img_, img_format_,
                                       GL_LINEAR);
  }

  std::shared_ptr<texture2d> create_shared_texture() {
    return std::make_shared<texture2d>(width_, height_, img_, img_format_,
                                       GL_LINEAR);
  }

private:
  int width_{}, height_{}, channels_{};
  GLenum img_format_;
  std::uint8_t *img_{nullptr};
};

#endif //CPP_FALLING_SAND_IMAGE_HPP
