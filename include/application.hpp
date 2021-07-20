//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_APPLICATION_HPP
#define CPP_FALLING_SAND_APPLICATION_HPP

#include <window.hpp>
#include <events/event.hpp>
#include <events/application_event.hpp>
#include <layer.hpp>
#include <memory>

#include <functional>
#include <graphics/sprite_render.hpp>

class application {
public:
  template<class... Args>
  static application *init(Args &&... args) {
    if (instance_ == nullptr) {
      instance_ = new application(args...);
    }
    return instance_;
  }

  static void deinit() {
    delete instance_;
  }

  void on_event(event &e) {
    dispatch_event<window_close_event>(e, [this](auto &&e) {
      return on_window_close(std::forward<decltype(e)>(e));
    });
    for (auto it = layers_.rbegin(); it != layers_.rend(); ++it) {
      if (e.handled) { break; }
      (*it)->on_event(e);
    }
  }

  void run() {
    running_ = true;
    while (running_) {
      for (auto l : layers_) {
        l->on_update();
      }
      window_.update();
    }
  }

  void close() { running_ = false; }

  void push_layer(layer *l) { layers_.push_back(l); }

  [[nodiscard]] static int window_width() { return instance_->window_.width(); }

  [[nodiscard]] static int
  window_height() { return instance_->window_.height(); }


  window *get_window() { return &(window_); }

  static application *instance() { return instance_; }

  static std::shared_ptr<sprite_render>
  basic_render() { return instance_->spriteRender_; }


private:
  template<class... Args>
  explicit application(Args... args)
      : window_(args...,
                [this](auto &&e) { on_event(std::forward<decltype(e)>(e)); }) {
    shader_ = std::make_shared<shader_program>(
        "resources/shaders/sprite2d.vert",
        "resources/shaders/sprite2d.frag");
    spriteRender_ = std::make_shared<sprite_render>(shader_,
                                                    window_.width(),
                                                    window_.height());
  }

  static inline application *instance_{nullptr};
  window window_;
  std::vector<layer *> layers_;
  bool running_{false};
  std::shared_ptr<sprite_render> spriteRender_;
  std::shared_ptr<shader_program> shader_;

  bool on_window_close(window_close_event &e) {
    close();
    return true;
  }
};

#endif //CPP_FALLING_SAND_APPLICATION_HPP
