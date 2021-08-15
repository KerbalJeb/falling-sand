//
// Created by ben on 2021-07-19.
//

#ifndef CPP_FALLING_SAND_KEY_EVENTS_HPP
#define CPP_FALLING_SAND_KEY_EVENTS_HPP

#include <events/event.hpp>

class key_down_event : public event {
public:
  // Takes the GLFW button ID and modifies (Shift, Alt, ect.) assosated with the event
  key_down_event(int buttonId, int modifiers) : buttonId_(buttonId),
                                                modifiers_(modifiers) {}

  // Returns the GLFW button ID for the event
  [[nodiscard]] int button_id() const { return buttonId_; }

  // Returns the GLFW modifier mask for the event
  [[nodiscard]] int modifiers() const { return modifiers_; }

  [[nodiscard]] event_type
  get_event_type() const override { return get_static_type(); }

  static event_type get_static_type() { return event_type::KeyPressed; }

private:
  int buttonId_, modifiers_;
};

#endif //CPP_FALLING_SAND_KEY_EVENTS_HPP
