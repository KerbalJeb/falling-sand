//
// Created by ben on 2021-07-19.
//

#ifndef CPP_FALLING_SAND_KEY_EVENTS_HPP
#define CPP_FALLING_SAND_KEY_EVENTS_HPP

#include <events/event.hpp>

class key_down_event : public event {
public:
  key_down_event(int buttonId, int modifiers) : buttonId_(buttonId),
                                                modifiers_(modifiers) {}

  int button_id() const { return buttonId_; }

  int modifiers() const { return modifiers_; }

  [[nodiscard]] event_type
  get_event_type() const override { return get_static_type(); }

  static event_type get_static_type() { return event_type::KeyPressed; }

private:
  int buttonId_, modifiers_;
};

#endif //CPP_FALLING_SAND_KEY_EVENTS_HPP
