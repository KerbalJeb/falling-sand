//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_IMAGE_HPP
#define CPP_FALLING_SAND_IMAGE_HPP

#include <filesystem>
#include <cassert>
#include <map>
#include <stb_image/stb_image.h>
#include <graphics/gl/texture2d.hpp>
#include <GLFW/glfw3.h>

class image {
public:
  explicit image(const std::filesystem::path &path) {
    img_ = stbi_load(path.c_str(), &width_, &height_, &channels_,
                     STBI_rgb_alpha);
    assert(img_ != nullptr);
  }

  image(int width, int height, int channels) : width_(width), height_(height),
                                               channels_(channels) {
    img_ = static_cast<uint8_t *>(std::malloc(width * height * channels));
    std::fill(img_, img_ + size(), 0);
  }

  ~image() {
    std::free(img_);
  }

  std::uint8_t *img_ptr() { return img_; }

  std::uint8_t &operator()(int x, int y) {
    return img_[(y * width_ + x) * channels_];
  }

  [[nodiscard]] const std::uint8_t *img_ptr() const { return img_; }

  [[nodiscard]] int width() const { return width_; }

  [[nodiscard]] int height() const { return height_; }

  [[nodiscard]] int channels() const { return channels_; }

  [[nodiscard]] int size() const { return width_ * height_ * channels_; }

  GLFWimage get_glfw_image() {
    return GLFWimage{width_, height_, img_};
  }

  texture2d create_texture() {
    return texture2d(width_, height_, img_, formatMapping.at(channels_),
                     GL_LINEAR);
  }

  std::unique_ptr<texture2d> create_unique_texture() {
    return std::make_unique<texture2d>(width_, height_, img_,
                                       formatMapping.at(channels_), GL_LINEAR);
  }

  std::shared_ptr<texture2d> create_shared_texture() {
    return std::make_shared<texture2d>(width_, height_, img_,
                                       formatMapping.at(channels_), GL_LINEAR);
  }

private:
  int width_{}, height_{}, channels_{};
  std::uint8_t *img_{nullptr};
  static const inline std::map<int, int> formatMapping{
      {3, GL_RGB},
      {4, GL_RGBA}
  };
};

#endif //CPP_FALLING_SAND_IMAGE_HPP
