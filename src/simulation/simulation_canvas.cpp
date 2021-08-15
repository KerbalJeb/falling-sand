#include <simulation/simulation_canvas.hpp>
#include <simulation/util.hpp>
#include <simulation/rn_generator.hpp>
#include <fstream>

simulation_canvas::simulation_canvas(int width, int height, element_manager em)
    : buffer((width + 2) * (height + 2), {0, 0, 0, 0}),
      width_(width + 2),
      height_(height + 2),
      elementManager_(std::move(em)) {
  fill_boundary();
}

void simulation_canvas::step_forward() {
  evenFrame = !evenFrame;
  // Swap update directions every frame to prevent odd artifacts
  int xInc = 1 - 2 * evenFrame;
  int xStart = evenFrame * width() - evenFrame;
  int xEnd = width() - xStart - 2 * evenFrame;

  for (int y = 0; y < height(); ++y) {
    for (int x = xStart; x != xEnd; x += xInc) {
      auto &p = get_particle(x, y);
      assert(p.id < elementManager_.size());
      if (!p || p.lastUpdated == evenFrame) { continue; }
      auto &e = elementManager_.get_element(p.id);

      p.lastUpdated = evenFrame;

      auto &lifetime = elementManager_.get_lifetime_rule(p.id);
      if (lifetime.transition_prob > 0 && lifetime.min_lifetime < p.age) {
        if (rn_gen.random_chance(lifetime.transition_prob)) {
          p = elementManager_.get_element(lifetime.new_element).create();
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
        auto &rule = elementManager_.get_contact_rule(p.id, neighbor.id);
        if (rn_gen.random_chance(rule.transform_chance)) {
          neighbor = elementManager_.get_element(rule.transform_element).create();
        }
      }

      switch (e.movement) {
        case movement_type::powder:
          move_powder(x, y, p, e);
          break;
        case movement_type::solid:
          break;
        case movement_type::liquid:
          move_liquid(x, y);
          break;
        case movement_type::gas:
          move_gas(x, y);
          break;
      }

      particle_instance *lastValid{&p};
      int i = p.id;
      bool finished =
          for_each_in_line(x, y, p.vx, p.vy, [&lastValid, this, i](int x, int y) {
            auto &p1 = get_particle(x, y);
            if (!elementManager_.get_displacement_rule(i, p1.id)) { return true; }
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
  assert(in_bounds(x, y));
  assert(id < elementManager_.size());
  b(*this, x, y, id);
}


void simulation_canvas::add_particle(int x, int y, element_id_type id) {
  get_particle(x, y) = elementManager_.get_element(id).create();
}

void simulation_canvas::clear() {
  std::fill(buffer.begin(), buffer.end(), particle_instance{});
  fill_boundary();
}

void simulation_canvas::write_to_image(image &img) {
  assert(img.height() == height());
  assert(img.width() == width());

  auto img_it = img.data();
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
  ++x;
  ++y;
  assert(in_bounds(x, y));
  return buffer[y * width_ + x];
}

const particle_instance &simulation_canvas::get_particle(int x, int y) const {
  ++x;
  ++y;
  assert(in_bounds(x, y));
  return buffer.at(y * width_ + x);
}

bool simulation_canvas::in_bounds(int x, int y) const {
  return x < width_ && y < height_ && y >= 0 && x >= 0;
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
  if (elementManager_.get_displacement_rule(p.id, neighbour.id)) {
    swap_particles(p, neighbour);
    return true;
  }
  if (elementManager_.get_displacement_rule(p.id, below.id)) {
    swap_particles(p, below);
    return true;
  }
  return false;
}

void simulation_canvas::move_liquid(int x, int y) {
  auto &below = get_particle(x, y + 1);
  auto &p = get_particle(x, y);
  auto &e = elementManager_.get_element(p.id);
  // Fall down
  if (elementManager_.get_displacement_rule(p.id, below.id)) {
    p.vy += gravity_accel;
    p.vx = 0;
    return;
  }

  // Move down and to the left or right
  auto dir = 2 * static_cast<int>(rn_gen.random_int(1)) - 1;
  auto &neighbour1 = get_particle(x + dir, y + 1);
  auto &neighbour2 = get_particle(x - dir, y + 1);

  if (elementManager_.get_displacement_rule(p.id, neighbour1.id)) {
    swap_particles(p, neighbour1);
    return;
  } else if (elementManager_.get_displacement_rule(p.id, neighbour2.id)) {
    swap_particles(p, neighbour2);
    return;
  }

  // Spread out
  int spread = e.opt_param;

  int xMin = x, xMax = x;

  while (xMin >= std::max(0, x - spread)) {
    auto &pNext = get_particle(xMin - 1, y);
    if (!elementManager_.get_displacement_rule(p.id, pNext.id)) { break; }
    --xMin;
  }

  while (xMax < std::min(width(), x + spread + 1)) {
    auto &pNext = get_particle(xMin + 1, y);
    if (!elementManager_.get_displacement_rule(p.id, pNext.id)) { break; }
    ++xMax;
  }

  for (int xNew = xMin; xNew < xMax; ++xNew) {
    int offset = rn_gen.random_int(xMax, xNew);
    auto &pNext = get_particle(offset, y);
    if (elementManager_.get_displacement_rule(p.id, pNext.id)) {
      swap_particles(p, pNext);
      x = xNew;
      // Check if we can fall down
      for (int yNew = y + 1; yNew < y + gravity_accel; ++yNew) {
        auto &pBelow = get_particle(x, yNew);
        if (elementManager_.get_displacement_rule(pNext.id, pBelow.id)) {
          swap_particles(pNext, pBelow);
        }
      }
      break;
    }
  }
}

void simulation_canvas::move_gas(int x, int y) {
  auto &p = get_particle(x, y);
  auto &e = elementManager_.get_element(p.id);
  int speed = 6;
  int vx = (int) (rn_gen.random_int(speed)) - speed / 2;
  int vy = (int) (rn_gen.random_int(speed)) - speed / 2 - (int) e.opt_param;
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

  int activeId = get_particle(0, 0).id;
  int count = 0;

  for (int y = 0; y < height(); ++y) {
    for (int x = 0; x < width(); ++x) {
      const auto &p = get_particle(x, y);
      if (p.id != activeId) {
        f << count << ":" << (unsigned int) activeId << ",";
        count = 0;
      }
      activeId = p.id;
      ++count;
    }
  }
  f << count << ":" << (unsigned int) activeId << std::endl;
  f.close();
  return true;
}

bool simulation_canvas::load(const std::filesystem::path &path) {
  std::ifstream f{path};
  if (!f.is_open()) { return false; }

  int width, height;
  f >> width >> height;

  if (width != this->width() || height != this->height()) { return false; }

  std::vector<element_id_type> ids;
  std::string line;
  std::string token_str;

  ids.reserve(width * height);
  auto max_id = elementManager_.size();
  while (f >> line) {
    std::stringstream s{line};

    while (std::getline(s, token_str, ',')) {
      std::stringstream token_stream{token_str};
      std::string count_str;
      std::string id_str;
      bool failed = false;
      failed |= !(bool) std::getline(token_stream, count_str, ':');
      failed |= !(bool) std::getline(token_stream, id_str, ':');

      if (failed) { return false; }

      char *end1;
      char *end2;
      errno = 0;
      auto count = std::strtol(count_str.c_str(), &end1, 10);
      auto int_id = std::strtol(id_str.c_str(), &end2, 10);

      if (*end2 != '\0' || *end1 != '\0' || int_id >= max_id || errno != 0) {
        return false;
      }
      for (int i = 0; i < count; ++i) {
        ids.push_back(int_id);
      }
    }
  }

  if (ids.size() != width * height) { return false; }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto &p = get_particle(x, y);
      auto i = ids[y * width + x];
      if (i >= elementManager_.size()) {
        return false;
      }
      p = elementManager_.get_element(i).create();
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

bool simulation_canvas::in_canvas(int x, int y) const {
  return x < width() && y < height() && y >= 0 && x >= 0;
}

void simulation_canvas::fill_boundary() {
  for (int x = -1; x <= width(); ++x) {
    get_particle(x, -1).id = elementManager_.boundary_id();
    get_particle(x, height()).id = elementManager_.boundary_id();
  }

  for (int y = -1; y <= height(); ++y) {
    get_particle(-1, y).id = elementManager_.boundary_id();
    get_particle(width(), y).id = elementManager_.boundary_id();
  }
}
