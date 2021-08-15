#include <application.hpp>

void application::generate_event(event &e) {
  dispatch_event<window_close_event>(e, [this](auto &&e) {
    return on_window_close(std::forward<decltype(e)>(e));
  });
  for (auto &layer :layers_) {
    if (e.handled) { break; }
    layer->on_event(e);
  }
}

void application::run() {
  running_ = true;
  while (running_) {
    for (auto l : layers_) {
      l->on_update();
    }
    window_.update();
  }
}
