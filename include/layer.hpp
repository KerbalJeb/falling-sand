
#ifndef CPP_FALLING_SAND_LAYER_HPP
#define CPP_FALLING_SAND_LAYER_HPP

#include <events/event.hpp>

// The layer virtual base class
// Can be inherited from to group update logic, event handling and render order
class layer {
public:
  layer() = default;

  virtual ~layer() = default;

  // Called by the application on every frame
  virtual void on_update() = 0;

  // Called by the application for every event
  virtual void on_event(event &e) = 0;
};

#endif //CPP_FALLING_SAND_LAYER_HPP
