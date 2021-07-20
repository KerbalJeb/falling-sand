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
  explicit ui_layer(std::shared_ptr<sprite_render> render)
      : render_(std::move(render)) {
    image img{"resources/img.png"};
    auto windowWidth = application::instance()->window_width();
    auto windowHeight = application::instance()->window_height();

    buttons.emplace_back("resources/img.png", 0, 0, 25, 75,
                         []() { std::cout << "Button Pressed" << std::endl; });
  }

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
  std::shared_ptr<sprite_render> render_;
};

#endif //CPP_FALLING_SAND_UI_LAYER_HPP
