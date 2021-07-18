//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_UI_BUTTON_HPP
#define CPP_FALLING_SAND_UI_BUTTON_HPP

#include <filesystem>
#include <functional>
#include <graphics/sprite.hpp>
#include <graphics/image.hpp>
#include <events/event.hpp>
#include <utility>

class ui_button {
public:
  using action = std::function<void()>;

  ui_button(const std::filesystem::path &img_path, int xPos, int yPos,
            int width, int height, action a) : onPressed_(std::move(a)) {
    image img{img_path};
    texture_ = img.create_shared_texture();
    sprite_.texture = texture_.get();
    sprite_.scale = glm::vec2(height, width);
    sprite_.pos = glm::vec2(xPos, yPos);
  }

  ui_button(const ui_button &) = default;

  ui_button(ui_button &&) = default;

  [[nodiscard]] const sprite &get_sprite() const { return sprite_; }

  bool on_event(event &e) {
    return dispatch_event<mouse_press_event>(e, [this](auto &&e) {
      return on_mouse_press(std::forward<decltype(e)>(e));
    });
  }

  void set_color(const glm::vec3 &newColor) {
    sprite_.color = newColor;
  }

private:
  sprite sprite_;
  std::shared_ptr<texture2d> texture_;
  action onPressed_;

  bool on_mouse_press(mouse_press_event &e) {
    double x, y;
    input::get_cursor(&x, &y);
    if (e.button_id() == GLFW_MOUSE_BUTTON_LEFT && in_button(x, y)) {
      onPressed_();
      e.handled = true;
      return true;
    }
    return false;
  }

  [[nodiscard]] bool in_button(float x, float y) const {
    float x0 = sprite_.pos.x, x1 = sprite_.pos.x + sprite_.scale.x;
    float y0 = sprite_.pos.y, y1 = sprite_.pos.y + sprite_.scale.y;

    return x >= x0 && x <= x1 && y >= y0 && y <= y1;
  }
};

#endif //CPP_FALLING_SAND_UI_BUTTON_HPP
