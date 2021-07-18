//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_EVENT_HPP
#define CPP_FALLING_SAND_EVENT_HPP

enum class event_type {
  None = 0,
  WindowClose,
  KeyPressed, KeyReleased, KeyTyped,
  MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

class event {
public:
  virtual ~event() = default;

  bool handled = false;

  virtual event_type get_event_type() const = 0;
};

template<class T, class F>
bool dispatch_event(event &e, const F &func) {
  if (e.get_event_type() == T::get_static_type()) {
    e.handled |= func(static_cast<T &>(e));
    return true;
  }
  return false;
}

#endif //CPP_FALLING_SAND_EVENT_HPP
