//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_UI_BUTTON_HPP
#define CPP_FALLING_SAND_UI_BUTTON_HPP

#include <filesystem>
#include <functional>
#include <graphics/sprite.hpp>
#include <graphics/image.hpp>
#include <utility>
#include <events/mouse_events.hpp>

// A basic UI button class
class ui_button {
public:
  using action = std::function<void()>;

  // Creates a button of width and height with an upper left corner at xPos,yPos
  // Renders the button using the img loaded from img_path (must be a valid image)
  // When clicked, the action function a will be called
  ui_button(const std::filesystem::path &img_path, int xPos, int yPos,
            int width, int height, action a);

  ui_button(const ui_button &) = default;

  ui_button(ui_button &&) = default;

  // Return a const reference to the get_sprite used for this button
  [[nodiscard]] const sprite &get_sprite() const { return sprite_; }

  // Should be called every time an event is received
  bool on_event(event &e);

  // Sets the button sprites color to newColor
  void set_color(const glm::vec3 &newColor) {
    sprite_.color = newColor;
  }

private:
  sprite sprite_;
  std::shared_ptr<texture2d> texture_;
  action onPressed_;

  bool on_mouse_press(mouse_press_event &e);

  [[nodiscard]] bool in_button(float x, float y) const;
};

#endif //CPP_FALLING_SAND_UI_BUTTON_HPP
