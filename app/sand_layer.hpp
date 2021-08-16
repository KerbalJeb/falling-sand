#ifndef CPP_FALLING_SAND_SANDBOX_HPP
#define CPP_FALLING_SAND_SANDBOX_HPP

#include <layer.hpp>
#include <input/input.hpp>
#include <graphics/sprite_render.hpp>
#include <simulation/simulation_canvas.hpp>
#include <simulation/brushes.hpp>
#include <utility>


class sand_layer : public layer {
public:
  sand_layer(int width, int height, int scale,
             std::shared_ptr<sprite_render> render)
      : w_(application::instance()->get_window()), width_(width), height_(height), scale_(scale),
        textureImage_(width, height, 3),
        canvas_(width, height, {
            {"empty_id",  movement_type::solid,  0,   0,   0,   0,    {0,   0,   {}},         {}},
            {"boundary",  movement_type::solid,  0,   0,   0,   0,    {0,   0,   {}},         {}},
            {"sand",      movement_type::powder, 194, 178, 128, 0.2,  {0,   0,   {}},         {}},
            {"salt",      movement_type::powder, 180, 180, 180, 0.1,  {0,   0,   {}},         {}},
            {"oil",       movement_type::liquid, 120, 103, 33,  0.05, {0,   0,   {}},         {}, 8},
            {"wall",      movement_type::solid,  77,  77,  77,  0,    {0,   0,   {}},         {}},
            {"lava",      movement_type::liquid, 170, 68,  0,   0.5,  {0,   0,   {}},         {
                                                                                                  {"water", "steam",    0.8},
                                                                                                  {"slt-water", "steam", 0.8},
                                                                                                  {"wood",      "fire",      0.05},
                                                                                                  {"oil",   "fire",      0.75},
                                                                                              },  1},

            {"steam",     movement_type::gas,    42,  127, 255, 0.1,  {200, 100, "water"},    {
                                                                                                  {"fire",  "empty_id", 0.15},
                                                                                              },  2},

            {"water",     movement_type::liquid, 75,  75,  200, 0.2,  {0,   0,   {}},         {
                                                                                                  {"fire",  "empty_id", 0.75},
                                                                                                  {"lava",      "sand",  0.05},
                                                                                                  {"salt",      "slt-water", 0.05},
                                                                                              },  5},
            {"slt-water", movement_type::liquid, 50,  50,  150, 0.2,  {0,   0,   {}}, {
                                                                                          {"fire", "empty_id", 0.75},
                                                                                          {"lava", "sand", 0.05},
                                                                                          {"salt", "slt-water", 0.01},
                                                                                          {"water", "slt-water", 0.002},
                                                                                          {"wood", "empty_id", 0.002},
                                                                                      },  4},

            {"wood",      movement_type::solid,  120, 70,  30,  0.1,  {0,   0,   {}},         {
                                                                                                  {"water", "wood",     0.01},
                                                                                              }},

            {"fire",      movement_type::gas,    255, 127, 42,  0.5,  {30,  30,  "empty_id"}, {   {"wood",  "fire",     0.1},
                                                                                                  {"water",     "steam", 0.5},
                                                                                                  {"slt-water", "steam",     0.5},
                                                                                                  {"oil",   "fire",      0.5},
                                                                                                  {"sand", "lava", 0.005},
                                                                                              }}
        }),
        render_(std::move(render)) {
    texture_ = std::make_unique<texture2d>(width, height,
                                           textureImage_.data());
    background_.texture = texture_.get();
    background_.scale = glm::vec2(width * scale, height * scale);
    background_.pos = glm::vec2(0);
    activeBrush_ =
        std::make_unique<rectangle_brush>(5, false);
    change_brush_size(5);
    activeId_ = canvas_.get_element_manager().find_id("sand");
  }

  void set_active_element(element_id_type id) {
    activeId_ = id;
  }

  void toggle_simulation() {
    simActive_ = !simActive_;
  }

  void on_update() override {
    double x, y;
    input::get_cursor_pos(&x, &y);
    on_mouse_event(x, y);

    if (simActive_) {
      canvas_.step_forward();
    }
    canvas_.write_to_image(textureImage_);
    texture_->update(textureImage_.data());
    render_->draw(background_);
  }

  void on_event(event &e) override {
    dispatch_event<key_down_event>(e, [this](auto &e) {
      return on_key(std::forward<decltype(e)>(e));
    });

    dispatch_event<mouse_press_event>(e, [this](auto &e) {
      double x, y;
      input::get_cursor_pos(&x, &y);
      return on_mouse_event(x, y);
    });

    dispatch_event<mouse_moves_event>(e, [this](auto &e) {
      return on_mouse_event(e.get_x(), e.get_y());
    });

    dispatch_event<drop_event>(e, [this](auto &e) {
      return on_drop(e);
    });
  }

  void change_brush_size(int newSize) {
    activeBrush_->set_radius(newSize);
    auto size = std::max(activeBrush_->size() * 2 * scale_, scale_);
    image newImg{size, size, 4};
    activeBrush_->draw_outline(newImg, scale_);
    auto glfw = newImg.get_glfw_image();
    w_.set_cursor(&glfw);
  }

  [[nodiscard]] const simulation_canvas &get_canvas() const { return canvas_; }

private:
  int width_, height_, scale_;
  image textureImage_;
  simulation_canvas canvas_;
  std::unique_ptr<texture2d> texture_;
  sprite background_;
  std::shared_ptr<sprite_render> render_;
  std::unique_ptr<rectangle_brush> activeBrush_;
  element_id_type activeId_;
  bool simActive_{true};
  window &w_;

  bool on_mouse_event(int x, int y) {
    int xcord = x / scale_;
    int ycord = y / scale_;
    bool status = false;
    if (canvas_.in_canvas(xcord, ycord)) {
      if (input::get_mouse_button(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        activeBrush_->set_overwrite(false);
        canvas_.add_particle(xcord, ycord, activeId_, *activeBrush_);
        status = true;
      }
      if (input::get_mouse_button(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        activeBrush_->set_overwrite(true);
        canvas_.add_particle(xcord, ycord, 0, *activeBrush_);
        status = true;
      }
      w_.use_custom_cursor();
    } else {
      w_.use_standard_cursor();
    }
    return status;
  }

  bool on_key(key_down_event &e) {
    switch (e.button_id()) {
      case GLFW_KEY_SPACE:
        toggle_simulation();
        break;
      case GLFW_KEY_DELETE:
        canvas_.clear();
        break;
      case GLFW_KEY_LEFT_BRACKET:
        change_brush_size(activeBrush_->size() -
                          std::clamp(activeBrush_->size() / 5, 1, 20));
        break;
      case GLFW_KEY_RIGHT_BRACKET:
        change_brush_size(activeBrush_->size() +
                          std::clamp(activeBrush_->size() / 5, 1, 20));
        break;
      case GLFW_KEY_S:
        if (e.modifiers() & GLFW_MOD_CONTROL) {
          canvas_.save();
          std::cout << "Saving..." << std::endl;
          break;
        }
      default:
        return false;
    }
    return true;
  }

  bool on_drop(drop_event &e) {
    auto &path = e.file_paths()[0];
    if (canvas_.load(path)) {
      std::cout << "Loaded Save" << std::endl;
    } else {
      std::cout << "Invalid Save File" << std::endl;
    }
    return true;
  }
};

#endif //CPP_FALLING_SAND_SANDBOX_HPP
