//get_event_type
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_APPLICATION_EVENT_HPP
#define CPP_FALLING_SAND_APPLICATION_EVENT_HPP

#include <events/event.hpp>

class window_close_event : public event {
public:
  window_close_event() = default;

  [[nodiscard]] event_type
  get_event_type() const override { return get_static_type(); }

  static event_type get_static_type() { return event_type::WindowClose; }
};

#endif //CPP_FALLING_SAND_APPLICATION_EVENT_HPP
