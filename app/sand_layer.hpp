//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_SANDBOX_HPP
#define CPP_FALLING_SAND_SANDBOX_HPP

#include <layer.hpp>
#include <input/input.hpp>
#include <graphics/sprite_render.hpp>
#include <simulation/simulation_canvas.hpp>
#include <utility>


class sand_layer : public layer {
public:
  sand_layer(int width, int height, int scale,
             std::shared_ptr<sprite_render> render)
      : width_(width), height_(height),
        windowHeight_(application::window_height()),
        windowWidth_(application::window_width()),
        textureImage(width, height, 3), canvas(width, height),
        render_(std::move(render)) {
    texture_ = std::make_unique<texture2d>(width, height,
                                           textureImage.img_ptr());
    background.texture = texture_.get();
    background.scale = glm::vec2(width * scale, height * scale);
    background.pos = glm::vec2(0);
  }

  void set_active_element(element_id_type id) {
    activeId_ = id;
  }

  void toggle_simulation() {
    simActive_ = !simActive_;
  }

  void on_update() override {
    if (input::get_mouse_button(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double xpos, ypos;
      input::get_cursor(&xpos, &ypos);
      double x = (xpos / windowWidth_);
      double y = (ypos / windowHeight_);

      int xcord = static_cast<int>(x * width_);
      int ycord = static_cast<int>(y * height_);

      xcord = std::clamp(xcord, 0, width_ - 1);
      ycord = std::clamp(ycord, 0, height_ - 1);

      canvas.add_particle(xcord, ycord, activeId_);
    }
    if (simActive_) {
      canvas.step_forward();
    }
    canvas.write_to_img(textureImage);
    texture_->update(textureImage.img_ptr());
    render_->draw(background);
  }

  void on_event(event &e) override {

  }

private:
  int width_, height_;
  int windowWidth_, windowHeight_;
  image textureImage;
  simulation_canvas canvas;
  std::unique_ptr<texture2d> texture_;
  sprite background;
  std::shared_ptr<sprite_render> render_;
  element_id_type activeId_{1};
  bool simActive_{true};
};

#endif //CPP_FALLING_SAND_SANDBOX_HPP
