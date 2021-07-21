#include <simulation/simulation_canvas.hpp>
#include <simulation/util.hpp>
#include <simulation/rn_generator.hpp>

simulation_canvas::simulation_canvas(int width, int height)
    : buffer((width + 1) * (height + 1), {0, 0, 0, 0}),
      width_(width + 1),
      height_(height + 1),
      boundaryId_(element_manager::instance().get_idx("boundary")) {
  // Fill the outside layer with special element to avoid boundary checks
  for (int x = 0; x < width_; ++x) {
    get_particle(x, 0).id = boundaryId_;
    get_particle(x, height_ - 1).id = boundaryId_;
  }

  for (int y = 0; y < height_; ++y) {
    get_particle(0, y).id = boundaryId_;
    get_particle(width_ - 1, y).id = boundaryId_;
  }
}

void simulation_canvas::step_forward() {
  evenFrame = !evenFrame;
  auto &em = element_manager::instance();

  for (int y = 1; y < height(); ++y) {
    for (int x = 1; x < width(); ++x) {
      auto &p = get_particle(x, y);
      assert(p.id < element_manager::instance().size());
      if (!p.id || p.lastUpdated == evenFrame) { continue; }
      auto &e = em.get_element(p.id);

      p.lastUpdated = evenFrame;
      // Update vel
      p.vy += gravity_accel;
      auto &below = get_particle(x, y + 1);
      if (below.id) {
        auto offset = 2 * rng::instance().random_chance_fast(2) - 1;
        auto &other = get_particle(x + offset, y + 1);
        if (!other.id) { std::swap(p, other); }
      }

      particle_instance *lastValid{nullptr};
      bool
          finished =
          for_each_in_line(x, y, p.vx, p.vy, [&lastValid, this](int x, int y) {
            auto &p = get_particle(x, y);
            if (p.id) { return true; }
            lastValid = &p;
            return false;
          });

      if (!finished) { p.vy = 0; }
      if (lastValid) { swap_particles(p, *lastValid); }

      // Apply rules
//      if (!e.update) { continue; }
//      em.get_element(p.id).update(*this, x, y);
    }
  }
}

void simulation_canvas::add_particle(int x, int y, element_id_type id,
                                     const simulation_canvas::brush &b) {
  assert(in_canvas(x, y));
  assert(id < element_manager::instance().size());
  b(*this, x, y, id);
}

void simulation_canvas::clear() {
  std::fill(buffer.begin(), buffer.end(), particle_instance{});
  for (int x = 0; x < width_; ++x) {
    get_particle(x, 0).id = boundaryId_;
    get_particle(x, height_ - 1).id = boundaryId_;
  }

  for (int y = 0; y < height_; ++y) {
    get_particle(0, y).id = boundaryId_;
    get_particle(width_ - 1, y).id = boundaryId_;
  }
}

void simulation_canvas::write_to_img(image &img) {
  assert(img.height() == height());
  assert(img.width() == width());

  auto img_it = img.img_ptr();
  for (int y = 0; y < height(); ++y) {
    for (int x = 0; x < width(); ++x) {
      const auto &p = get_particle(x, y);
      img_it[0] = p.r;
      img_it[1] = p.g;
      img_it[2] = p.b;
      img_it += 3;
    }
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

void simulation_canvas::move_powder(int x, int y) {

}

void simulation_canvas::move_fluid(int x, int y) {

}

void simulation_canvas::move_gas(int x, int y) {

}
