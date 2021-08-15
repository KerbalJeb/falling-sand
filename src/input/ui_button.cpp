#include <input/ui_button.hpp>
#include <input/input.hpp>

ui_button::ui_button(const std::filesystem::path &img_path, int xPos, int yPos, int width, int height,
                     ui_button::action a) : onPressed_(std::move(a)) {
  image img{img_path};
  texture_ = img.create_shared_texture();
  sprite_.texture = texture_.get();
  sprite_.scale = glm::vec2(height, width);
  sprite_.pos = glm::vec2(xPos, yPos);
}

bool ui_button::on_event(event &e) {
  return dispatch_event<mouse_press_event>(e, [this](auto &&e) {
    return on_mouse_press(std::forward<decltype(e)>(e));
  });
}

bool ui_button::on_mouse_press(mouse_press_event &e) {
  double x, y;
  input::get_cursor_pos(&x, &y);
  if (e.button_id() == GLFW_MOUSE_BUTTON_LEFT && in_button(x, y)) {
    onPressed_();
    e.handled = true;
    return true;
  }
  return false;
}

bool ui_button::in_button(float x, float y) const {
  float x0 = sprite_.pos.x, x1 = sprite_.pos.x + sprite_.scale.x;
  float y0 = sprite_.pos.y, y1 = sprite_.pos.y + sprite_.scale.y;

  return x >= x0 && x <= x1 && y >= y0 && y <= y1;
}
