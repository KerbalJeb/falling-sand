//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_MOUSE_EVENTS_HPP
#define CPP_FALLING_SAND_MOUSE_EVENTS_HPP

#include <events/event.hpp>

class mouse_press_event : public event {
public:
  explicit mouse_press_event(int button, int mods) : button_(button),
                                                     mods_(mods) {}


  [[nodiscard]] int button_id() const { return button_; }

  [[nodiscard]] int mods() const { return mods_; }

  [[nodiscard]] event_type
  get_event_type() const override { return get_static_type(); }

  static event_type get_static_type() { return event_type::MouseButtonPressed; }

private:
  int button_, mods_;
};

#endif //CPP_FALLING_SAND_MOUSE_EVENTS_HPP
