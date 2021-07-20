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
  explicit ui_layer(std::shared_ptr<sprite_render> render,
                    std::vector<ui_button> buttons)
      : render_(std::move(render)), buttons_(std::move(buttons)) {}

  void on_update() override {
    for (const auto &b:buttons_) { render_->draw(b.get_sprite()); }
  }

  void on_event(event &e) override {
    for (auto &b:buttons_) { b.on_event(e); }
  }

private:
  std::vector<ui_button> buttons_;
  std::shared_ptr<sprite_render> render_;
};

#endif //CPP_FALLING_SAND_UI_LAYER_HPP
