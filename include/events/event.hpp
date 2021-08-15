
#ifndef CPP_FALLING_SAND_EVENT_HPP
#define CPP_FALLING_SAND_EVENT_HPP

enum class event_type {
  None = 0,
  WindowClose,
  KeyPressed, KeyReleased, KeyTyped,
  MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
  PathDrop
};

// \brief The abstract base event class
// All events need to inherit from this class
// Events must also implement a static get_static_type function that
// returns the appropriate event_type enum value
// get_event_type() should also return this value
class event {
public:
  virtual ~event() = default;

  bool handled = false;

  [[nodiscard]] virtual event_type get_event_type() const = 0;
};

// Calls an event handler function if the event type matches the template parameter
// Sets e.handled to true if the event has been handled
// T is The event type to listen for
// e is the event to handle process
// func is the event handler function, takes an event
// and returns true if the event was handled and false otherwise
// Returns true if the event was handled, false otherwise
template<class T, class F>
bool dispatch_event(event &e, const F &func) {
  if (e.get_event_type() == T::get_static_type()) {
    e.handled |= func(static_cast<T &>(e));
    return e.handled;
  }
  return false;
}

#endif //CPP_FALLING_SAND_EVENT_HPP
