#ifndef CPP_FALLING_SAND_DROP_EVENT_HPP
#define CPP_FALLING_SAND_DROP_EVENT_HPP

#include <events/event.hpp>

class drop_event : public event {
public:
  drop_event(std::vector <std::string> &&paths) : paths_(std::move(paths)) {}

  [[nodiscard]] event_type
  get_event_type() const override { return get_static_type(); }

  static event_type get_static_type() { return event_type::PathDrop; }

  const std::vector <std::string> &get_paths() const { return paths_; }

private:
  std::vector <std::string> paths_;
};

#endif //CPP_FALLING_SAND_DROP_EVENT_HPP
