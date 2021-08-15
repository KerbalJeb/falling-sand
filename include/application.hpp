//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_APPLICATION_HPP
#define CPP_FALLING_SAND_APPLICATION_HPP

#include <ranges>
#include <window.hpp>
#include <events/event.hpp>
#include <events/application_events.hpp>
#include <layer.hpp>
#include <memory>

#include <functional>
#include <graphics/sprite_render.hpp>

// A singleton class used to manage the running application
// Handles initialization and events
class application {
public:
  // Initialize the application
  // Provide a path to the vertex and fragment shaders to use in the sprite render
  // Along with the arguments to pass to the window constructor
  template<class... Args>
  static application *
  init(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path, Args &&... args) {
    if (!glfwInit()) {
      std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback([](int id, auto description) {
      std::cout << "Error(" << id << "): " << description << std::endl;
    });

    if (instance_ == nullptr) {
      instance_ = new application(vertex_path, fragment_path, args...);
    }
    return instance_;
  }

  static void deinit() {
    delete instance_;
  }

  // Generates the event passed to it by passing it along to all layers in the application
  void generate_event(event &e);

  // Start running the application
  void run();

  // Stop the application
  void close() { running_ = false; }

  void push_layer(layer *l) { layers_.push_back(l); }

  // Get the window used by the application
  window &get_window() { return window_; }

  // The singleton instance
  static application *instance() { return instance_; }

  // The sprite render
  static std::shared_ptr<sprite_render>
  basic_render() { return instance_->spriteRender_; }


private:
  template<class... Args>
  explicit
  application(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path, Args... args)
      : window_(args...,
                [this](auto &&e) { generate_event(std::forward<decltype(e)>(e)); }) {
    shader_ = std::make_shared<shader_program>(
        vertex_path,
        fragment_path);
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
