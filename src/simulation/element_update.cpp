#include <simulation/element_update.hpp>

void
elements::update_sand::operator()(simulation_canvas &canvas, int x,
                                  int y) const {
  auto &p = canvas.get_particle(x, y);

  if (canvas.on_floor(y) || canvas.get_particle(x, y + 1).id) {
    p.vy = 0;
    if (canvas.in_canvas(x - 1, y + 1)) {
      auto &below_left = canvas.get_particle(x - 1, y + 1);
      if (!below_left.id) {
        std::swap(below_left, p);
        return;
      }
    }

    if (canvas.in_canvas(x + 1, y + 1)) {
      auto &below_right = canvas.get_particle(x + 1, y + 1);
      if (!below_right.id) {
        std::swap(below_right, p);
        return;
      }
    }
    return;
  }

  p.vy += gravity_accel;

  int nextX = x + p.vx, nextY = y + p.vy;
  auto &pn = find_in_line(canvas, x, nextX, y, nextY, [](const auto &p0) {
    return p0.id == 0;
  });

  std::swap(pn, p);
}

void elements::update_water::operator()(simulation_canvas &canvas, int x,
                                        int y) const {
  auto &p = canvas.get_particle(x, y);

  if (canvas.on_floor(y) || canvas.get_particle(x, y + 1).id) {
    p.vy = 0;
    if (canvas.in_canvas(x - 1, y + 1)) {
      auto &below_left = canvas.get_particle(x - 1, y + 1);
      if (!below_left.id) {
        std::swap(below_left, p);
        return;
      }
    }

    if (canvas.in_canvas(x + 1, y + 1)) {
      auto &below_right = canvas.get_particle(x + 1, y + 1);
      if (!below_right.id) {
        std::swap(below_right, p);
        return;
      }
    }

    if (canvas.in_canvas(x + 1, y)) {
      auto &right = canvas.get_particle(x + 1, y);
      if (!right.id) {
        std::swap(right, p);
        return;
      }
    }

    if (canvas.in_canvas(x - 1, y)) {
      auto &left = canvas.get_particle(x - 1, y);
      if (!left.id) {
        std::swap(left, p);
        return;
      }
    }
    return;
  }

  p.vy += gravity_accel;

  int nextX = x + p.vx, nextY = y + p.vy;
  auto &pn = find_in_line(canvas, x, nextX, y, nextY, [](const auto &p0) {
    return p0.id == 0;
  });

  std::swap(pn, p);
}
