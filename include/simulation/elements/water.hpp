//
// Created by ben on 2021-07-19.
//

#ifndef CPP_FALLING_SAND_WATER_HPP
#define CPP_FALLING_SAND_WATER_HPP

#include <simulation/util.hpp>

namespace elements {
  struct update_water {
    void operator()(simulation_canvas &canvas, int x, int y) {
      auto &p = canvas.get_particle(x, y);

      if (canvas.on_floor(y) ||
          canvas.get_particle(x, y + 1).id != element::empty_id()) {
        p.vy = 0;
        if (canvas.in_canvas(x - 1, y + 1)) {
          auto &below_left = canvas.get_particle(x - 1, y + 1);
          if (below_left.id == element::empty_id()) {
            std::swap(below_left, p);
            return;
          }
        }

        if (canvas.in_canvas(x + 1, y + 1)) {
          auto &below_right = canvas.get_particle(x + 1, y + 1);
          if (below_right.id == element::empty_id()) {
            std::swap(below_right, p);
            return;
          }
        }

        if (canvas.in_canvas(x + 1, y)) {
          auto &right = canvas.get_particle(x + 1, y);
          if (right.id == element::empty_id()) {
            std::swap(right, p);
            return;
          }
        }

        if (canvas.in_canvas(x - 1, y)) {
          auto &left = canvas.get_particle(x - 1, y);
          if (left.id == element::empty_id()) {
            std::swap(left, p);
            return;
          }
        }
        return;
      }

      p.vy += gravity_accel;

      int nextX = x + p.vx, nextY = y + p.vy;
      auto &pn = find_in_line(canvas, x, nextX, y, nextY, [](const auto &p0) {
        return p0.id == element::empty_id();
      });

      std::swap(pn, p);
    }
  };
}

#endif //CPP_FALLING_SAND_WATER_HPP
