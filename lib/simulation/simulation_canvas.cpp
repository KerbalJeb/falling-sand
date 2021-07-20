#include <simulation/simulation_canvas.hpp>

simulation_canvas::simulation_canvas(int width, int height)
    : buffer(width * height, {0, 0, 0, 0}), width_(width),
      height_(height) {}

void simulation_canvas::add_particle(int x, int y, element_id_type id,
                                     const simulation_canvas::brush &b) {
  assert(in_canvas(x, y));
  assert(id < element_manager::instance().size());
  b(*this, x, y, id);
}

void simulation_canvas::clear() {
  std::fill(buffer.begin(), buffer.end(), particle_instance{});
}

void simulation_canvas::step_forward() {
  evenFrame ^= 1;
  int xStart = evenFrame ? 0 : width_ - 1;
  int xEnd = evenFrame ? width_ : -1;
  int xInc = evenFrame ? 1 : -1;
  auto &em = element_manager::instance();

  for (int y = height_ - 1; y >= 0; --y) {
    for (int x = xStart; x != xEnd; x += xInc) {
      auto &p = get_particle(x, y);
      assert(p.id < element_manager::instance().size());
      if (p.id != 0 && p.lastUpdated != evenFrame) {
        p.lastUpdated = evenFrame;
        em.get_element(p.id).update(*this, x, y);
      }
    }
  }
}

void simulation_canvas::write_to_img(image &img) {
  assert(img.height() == height_);
  assert(img.width() == width_);

  auto img_it = img.img_ptr();
  for (const auto &e:buffer) {
    img_it[0] = e.r;
    img_it[1] = e.g;
    img_it[2] = e.b;
    img_it += 3;
  }
}

particle_instance &simulation_canvas::get_particle(int x, int y) {
  assert(in_canvas(x, y));
  return buffer[y * width_ + x];
}

const particle_instance &simulation_canvas::get_particle(int x, int y) const {
  assert(in_canvas(x, y));
  return buffer.at(y * width_ + x);
}

bool simulation_canvas::in_canvas(int x, int y) const {
  return x < width_ && y < height_ && y >= 0 && x >= 0;
}

bool simulation_canvas::on_floor(int y) const {
  return y == height_ - 1;
}
