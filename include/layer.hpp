//
// Created by ben on 2021-07-17.
//

#ifndef CPP_FALLING_SAND_LAYER_HPP
#define CPP_FALLING_SAND_LAYER_HPP

#include <events/event.hpp>

class layer {
public:
  layer() = default;

  virtual ~layer() = default;

  virtual void on_update() = 0;

  virtual void on_event(event &e) = 0;
};

#endif //CPP_FALLING_SAND_LAYER_HPP
