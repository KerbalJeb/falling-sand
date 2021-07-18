//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_UI_LAYER_HPP
#define CPP_FALLING_SAND_UI_LAYER_HPP

#include <layer.hpp>
#include <graphics/sprite_render.hpp>
#include <graphics/image.hpp>
#include <input/ui_button.hpp>
#include <utility>

class ui_layer : public layer {
public:
  explicit ui_layer(std::shared_ptr<shader_program> shader) : shader_(
      std::move(shader)) {
    image img{"resources/img.png"};
    auto windowWidth = application::instance()->window_width();
    auto windowHeight = application::instance()->window_height();
    render_ = std::make_unique<sprite_render>(shader_, windowWidth,
                                              windowHeight);

    buttons.emplace_back("resources/img.png", 0, 0, 25, 75,
                         []() { std::cout << "Button Pressed" << std::endl; });
  }

  ui_layer() : ui_layer(
      std::make_shared<shader_program>("shaders/sprite2d.vert",
                                       "shaders/sprite2d.frag")) {}

  void on_update() override {
    for (const auto &b:buttons) {
      render_->draw(b.get_sprite());
    }
  }

  void on_event(event &e) override {
    for (auto &b:buttons) {
      b.on_event(e);
    }
  }

private:
  std::vector<ui_button> buttons;
  std::shared_ptr<shader_program> shader_;
  std::unique_ptr<sprite_render> render_;
};

#endif //CPP_FALLING_SAND_UI_LAYER_HPP
