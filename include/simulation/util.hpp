//
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_UTIL_HPP
#define CPP_FALLING_SAND_UTIL_HPP

#include <random>
#include <simulation/simulation_canvas.hpp>

// Executes f for each point on the line starting a (x,y)
// and ending at (x+dx,y+dy) or when func returns true
template<class F>
bool for_each_in_line(int x, int y, int dx, int dy, F func) {
  int xi = 1 - 2 * std::signbit(dx);
  int yi = 1 - 2 * std::signbit(dy);

  dx = std::abs(dx);
  dy = std::abs(dy);

  bool swapped = false;
  if (dy > dx) {
    std::swap(x, y);
    std::swap(dx, dy);
    std::swap(xi, yi);
    swapped = true;
  }
  auto E = 2 * dy - dx;
  auto A = 2 * dy;
  auto B = 2 * (dy - dx);
  bool done;
  for (int i = 0; i < dx; ++i) {
    if (E > 0) {
      y += yi;
      E += B;
    } else {
      E += A;
    }
    x += xi;
    if (swapped) { done = func(y, x); }
    else { done = func(x, y); }
    if (done) { return false; }
  }
  return true;
}

#endif //CPP_FALLING_SAND_UTIL_HPP
