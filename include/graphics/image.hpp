
#ifndef CPP_FALLING_SAND_IMAGE_HPP
#define CPP_FALLING_SAND_IMAGE_HPP

#include <filesystem>
#include <cassert>
#include <map>
#include <stb_image/stb_image.h>
#include <graphics/gl/texture2d.hpp>
#include <GLFW/glfw3.h>

// Utility class for loading and creating images
class image {
public:
  // Loads an image from a file
  explicit image(const std::filesystem::path &path);

  // Creates an empty_id image of size width x height, with the specified number of channels
  image(int width, int height, int channels);

  ~image();

  // Return a raw pointer to the image data
  std::uint8_t *data() { return img_; }

  [[nodiscard]] const std::uint8_t *data() const { return img_; }

  // Get a poiter to the start of the pixel at x,y
  std::uint8_t *operator()(int x, int y) {
    return &img_[(y * width_ + x) * channels_];
  }


  // The width of the image
  [[nodiscard]] int width() const { return width_; }

  // The height of the image
  [[nodiscard]] int height() const { return height_; }

  // The number of channels in the image
  [[nodiscard]] int channels() const { return channels_; }

  // The total size of the image in bytes
  [[nodiscard]] int size() const { return width_ * height_ * channels_; }

  // Creates a glfw image from the image
  GLFWimage get_glfw_image() {
    return GLFWimage{width_, height_, img_};
  }

  // Creates an opengl texture from the image
  texture2d create_texture() {
    return texture2d(width_, height_, img_, formatMapping.at(channels_),
                     GL_LINEAR);
  }

  // Creates a shared pointer to an opengl texture from the image
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
