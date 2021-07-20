//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_ELEMENT_UPDATE_HPP
#define CPP_FALLING_SAND_ELEMENT_UPDATE_HPP

#include <simulation/util.hpp>

namespace elements {
  class update_sand {
  public:
    void operator()(simulation_canvas &canvas, int x, int y) const;
  };

  struct update_water {
    void operator()(simulation_canvas &canvas, int x, int y) const;
  };

}

#endif //CPP_FALLING_SAND_ELEMENT_UPDATE_HPP
