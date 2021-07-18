//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_SANDBOX_HPP
#define CPP_FALLING_SAND_SANDBOX_HPP

#include <layer.hpp>
#include <input/input.hpp>
#include <graphics/sprite_render.hpp>


class sand_layer : public layer {
public:
  sand_layer(int width, int height) : width_(width), height_(height) {
    const int bufferSize = width * height * 3;
    try {
      dataBuffer_ = new std::uint8_t[bufferSize];
      std::fill_n(dataBuffer_, bufferSize, 0);

      shader_ = std::make_shared<shader_program>("shaders/sprite2d.vert",
                                                 "shaders/sprite2d.frag");
      render_ = std::make_unique<sprite_render>(shader_, width, height);
      texture_ = std::make_unique<texture2d>(width, height, dataBuffer_);
      background.texture = texture_.get();
      background.scale = glm::vec2(width, height);
      background.pos = glm::vec2(0);
    }
    catch (...) {
      delete[] dataBuffer_;
      throw;
    }
  }

  ~sand_layer() override {
    delete[] dataBuffer_;
  }

  void on_update() override {
    if (input::get_mouse_button(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double xpos, ypos;
      input::get_cursor(&xpos, &ypos);
      double x = (xpos / 512);
      double y = (ypos / 512);

      int xcord = static_cast<int>(x * width_);
      int ycord = static_cast<int>(y * height_);

      xcord = std::clamp(xcord, 0, width_ - 1);
      ycord = std::clamp(ycord, 0, height_ - 1);
      int idx = (ycord * width_ + xcord) * 3;
      std::fill_n(&dataBuffer_[idx + 1], 2, 255);
      texture_->update(dataBuffer_);
      render_->draw(background);
    }
  }

  void on_event(event &e) override {

  }

private:
  int width_, height_;
  std::uint8_t *dataBuffer_;
  std::unique_ptr<texture2d> texture_;
  sprite background;
  std::shared_ptr<shader_program> shader_;
  std::unique_ptr<sprite_render> render_;

};

#endif //CPP_FALLING_SAND_SANDBOX_HPP
