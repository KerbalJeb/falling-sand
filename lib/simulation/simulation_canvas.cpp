#include <simulation/simulation_canvas.hpp>
#include <simulation/util.hpp>
#include <simulation/rn_generator.hpp>
#include <fstream>

simulation_canvas::simulation_canvas(int width, int height)
    : buffer((width + 2) * (height + 2), {0, 0, 0, 0}),
      width_(width + 2),
      height_(height + 2),
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

  for (int y = 1; y <= height(); ++y) {
    for (int x = 1; x <= width(); ++x) {
      auto &p = get_particle(x, y);
      assert(p.id < em.size());
      if (!p.id || p.lastUpdated == evenFrame) { continue; }
      auto &e = em.get_element(p.id);

      p.lastUpdated = evenFrame;

      auto &lifetime = em.get_lifetime_rule(p.id);
      if (lifetime.transition_prob > 0 && lifetime.min_lifetime < p.age) {
        if (rn_gen.random_chance(lifetime.transition_prob)) {
          p = em.get_element(lifetime.new_element).create();
          continue;
        }
      }
      p.age += 1;

      constexpr std::pair<int, int> neighbors_offsets[]{
          {1,  0},
          {-1, 0},
          {0,  -1},
          {0,  1}};

      for (auto &offset:neighbors_offsets) {
        auto &neighbor = get_particle(x + offset.first, y + offset.second);
        auto &rule = em.get_contact_rule(p.id, neighbor.id);
        if (rn_gen.random_chance(rule.transform_chance)) {
          neighbor = em.get_element(rule.transform_element).create();
        }
      }

      switch (e.movement) {
        case movement_type::powder:
          move_powder(x, y, p, e);
          break;
        case movement_type::solid:
          break;
        case movement_type::liquid:
          move_liquid(x, y, p, e);
          break;
        case movement_type::gas:
          move_gas(x, y, p, e);
          break;
      }

      particle_instance *lastValid{nullptr};
      int i = p.id;
      bool finished =
          for_each_in_line(x, y, p.vx, p.vy, [&lastValid, this, i](int x, int y) {
            auto &p1 = get_particle(x, y);
            if (!em.get_displacement_rule(i, p1.id)) { return true; }
            lastValid = &p1;
            return false;
          });

      if (!finished) { p.vy = 0; }
      if (lastValid) { swap_particles(p, *lastValid); }
    }
  }
}

void simulation_canvas::add_particle(int x, int y, element_id_type id,
                                     const simulation_canvas::brush &b) {
  assert(in_canvas(x, y));
  assert(id < element_manager::instance().size());
  b(*this, x, y, id);
}


void simulation_canvas::add_particle(int x, int y, element_id_type id) {
  get_particle(x, y) = em.get_element(id).create();
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

bool
simulation_canvas::move_powder(int x, int y, particle_instance &p,
                               const element &e) {
  auto &below = get_particle(x, y + 1);
  if (!below.id) {
    p.vy += gravity_accel;
    return true;
  }
  constexpr std::int16_t dirs[] = {-1, 1};
  const int rand_offset = static_cast<int>(rn_gen.random_int(1));

  auto dir = dirs[rand_offset];
  auto &neighbour = get_particle(x + dir, y + 1);
  if (em.get_displacement_rule(p.id, neighbour.id)) {
    swap_particles(p, neighbour);
    return true;
  }
  if (em.get_displacement_rule(p.id, below.id)) {
    swap_particles(p, below);
    return true;
  }
  return false;
}

void simulation_canvas::move_liquid(int x, int y, particle_instance &p, const element &e) {
  // todo: improve liquid movement rules
  auto &below = get_particle(x, y + 1);
  if (em.get_displacement_rule(p.id, below.id)) {
    p.vy += gravity_accel;
    p.vx = 0;
    return;
  }

  auto dir = 2 * static_cast<int>(rn_gen.random_int(1)) - 1;
  auto &neighbour1 = get_particle(x + dir, y + 1);
  auto &neighbour2 = get_particle(x - dir, y + 1);

  if (em.get_displacement_rule(p.id, neighbour1.id)) {
    swap_particles(p, neighbour1);
    return;
  } else if (em.get_displacement_rule(p.id, neighbour2.id)) {
    swap_particles(p, neighbour2);
    return;
  }

  auto &horizontalNeighbour1 = get_particle(x + dir, y);
  auto &horizontalNeighbour2 = get_particle(x - dir, y);
  int spread = 5;
  if (!horizontalNeighbour2.id) { p.vx = -spread * dir; }
}

void simulation_canvas::move_gas(int x, int y, particle_instance &p, const element &e) {
  int speed = 6;
  int vx = static_cast<int>(rn_gen.random_int(speed)) - speed / 2;
  int vy = static_cast<int>(rn_gen.random_int(speed)) - speed / 2;
  p.vy = vy;
  p.vx = vx;
}

bool simulation_canvas::save(const std::filesystem::path &path) const {
  std::ofstream f{path};
  assert(f.is_open());
  if (!f.is_open()) {
    return false;
  }

  f << width() << " " << height() << std::endl;

  for (int y = 1; y <= height(); ++y) {
    for (int x = 1; x <= width(); ++x) {
      const auto &p = get_particle(x, y);
      f << (unsigned int) p.id;
      if (x < width()) { f << ","; }
    }
    f << std::endl;
  }
  f.close();
  return true;
}

bool simulation_canvas::load(const std::filesystem::path &path) {
  std::ifstream f{path};
  assert(f.is_open());
  if (!f.is_open()) { return false; }

  int width, height;
  f >> width >> height;
  assert(f.is_open());
  assert(width == this->width());
  assert(height == this->height());
  std::vector<element_id_type> ids;
  std::string line;
  std::string id_str;

  ids.reserve(width * height);
  while (f.good()) {
    std::getline(f, line);
    std::stringstream s{line};
    auto max_id = em.size();


    while (std::getline(s, id_str, ',')) {
      char *end;
      errno = 0;
      auto int_id = std::strtol(id_str.c_str(), &end, 10);

      if (*end != '\0' || int_id >= max_id || errno != 0) {
        return false;
      }
      ids.push_back(int_id);
    }
  }
  assert(ids.size() == width * height);
  if (ids.size() != width * height) { return false; }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto &p = get_particle(x + 1, y + 1);
      auto i = ids[y * width + x];
      p = em.get_element(i).create();
    }
  }

  return true;
}

bool simulation_canvas::save() const {
  std::string filename;
  int save_idx = 0;

  while (std::filesystem::exists(filename = create_save_file_name(save_idx))) {
    ++save_idx;
    // Only keep 100 saves and override last save if over 100
    if (save_idx >= 100) {
      break;
    }
  }
  return save(filename);
}

std::string simulation_canvas::create_save_file_name(int save_idx) const {
  auto size_s = std::snprintf(
      nullptr,
      0,
      "%s-%d.%s",
      save_file_name_.c_str(),
      save_idx,
      save_file_ext_.c_str());
  ++size_s;
  assert(size_s > 0);
  auto buf = std::make_unique<char[]>(size_s);
  std::snprintf(buf.get(), size_s, "%s-%d.%s",
                save_file_name_.c_str(),
                save_idx,
                save_file_ext_.c_str());
  return std::string{buf.get(), buf.get() + size_s - 1};
}
