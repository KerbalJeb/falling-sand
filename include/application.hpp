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

class application {
public:
  static application *
  init(int width, int height, const std::string &title, GLboolean resizable) {
    if (instance_ == nullptr) {
      instance_ = new application(width, height, title, resizable);
    }
    return instance_;
  }

  static application *
  init(int width, int height, const std::string &title, GLboolean resizable,
       const std::filesystem::path &path) {
    if (instance_ == nullptr) {
      instance_ = new application(width, height, title, resizable, path);
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


private:
  application(int width, int height, const std::string &title,
              GLboolean resizable)
      : window_(width, height, title, resizable,
                [this](auto &&e) {
                  on_event(std::forward<decltype(e)>(e));
                }) {}

  application(int width, int height, const std::string &title,
              GLboolean resizable, const std::filesystem::path &path)
      : window_(width, height, title, resizable,
                [this](auto &&e) {
                  on_event(std::forward<decltype(e)>(e));
                }, path) {}

  static inline application *instance_{nullptr};
  window window_;
  std::vector<layer *> layers_;
  bool running_{false};

  bool on_window_close(window_close_event &e) {
    close();
    return true;
  }
};

#endif //CPP_FALLING_SAND_APPLICATION_HPP
