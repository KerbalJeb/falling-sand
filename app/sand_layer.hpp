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
      : width_(width), height_(height), scale_(scale),
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
      double x, y;
      input::get_cursor(&x, &y);
      on_mouse_event(x, y);
    }
    if (simActive_) {
      canvas.step_forward();
    }
    canvas.write_to_img(textureImage);
    texture_->update(textureImage.img_ptr());
    render_->draw(background);
  }

  void on_event(event &e) override {
    dispatch_event<key_down_event>(e, [this](auto &e) {
      return on_key(std::forward<decltype(e)>(e));
    });

    dispatch_event<mouse_press_event>(e, [this](auto &e) {
      if (e.button_id() == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        input::get_cursor(&x, &y);
        return on_mouse_event(x, y);
      }
      return false;
    });

    dispatch_event<mouse_moves_event>(e, [this](auto &e) {
      if (input::get_mouse_button(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        return on_mouse_event(e.get_x(), e.get_y());
      }
      return false;
    });
  }

private:
  int width_, height_, scale_;
  image textureImage;
  simulation_canvas canvas;
  std::unique_ptr<texture2d> texture_;
  sprite background;
  std::shared_ptr<sprite_render> render_;
  element_id_type activeId_{1};
  bool simActive_{true};

  bool on_mouse_event(int x, int y) {
    int xcord = x / scale_;
    int ycord = y / scale_;

    if (canvas.in_canvas(xcord, ycord)) {
      canvas.add_particle(xcord, ycord, activeId_);
      return true;
    }
    return false;
  }

  bool on_key(key_down_event &e) {
    switch (e.button_id()) {
      case GLFW_KEY_SPACE:
        toggle_simulation();
        break;
      case GLFW_KEY_DELETE:
        canvas.clear();
      default:
        return false;
    }
    return true;
  }
};

#endif //CPP_FALLING_SAND_SANDBOX_HPP
