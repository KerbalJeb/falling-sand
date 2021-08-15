#ifndef CPP_FALLING_SAND_DROP_EVENT_HPP
#define CPP_FALLING_SAND_DROP_EVENT_HPP

#include <events/event.hpp>
#include <vector>
#include <string>

// A file drop window event
class drop_event : public event {
public:
  // Takes a vector of strings corresponding to the paths of the file dropped
  explicit drop_event(std::vector<std::string> &&paths) : paths_(std::move(paths)) {}

  [[nodiscard]] event_type
  get_event_type() const override { return get_static_type(); }

  static event_type get_static_type() { return event_type::PathDrop; }

  // Returns all the file paths associated with this event
  [[nodiscard]] const std::vector<std::string> &file_paths() const { return paths_; }

private:
  std::vector<std::string> paths_;
};

#endif //CPP_FALLING_SAND_DROP_EVENT_HPP
