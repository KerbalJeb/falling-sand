///tmp/cpp-project-build
// Created by ben on 2021-07-18.
//

#ifndef CPP_FALLING_SAND_SIMULATION_CANVAS_HPP
#define CPP_FALLING_SAND_SIMULATION_CANVAS_HPP

#include <utility>
#include <vector>
#include <initializer_list>
#include <random>
#include <simulation/particle.hpp>
#include <simulation/element.hpp>
#include <graphics/image.hpp>

class simulation_canvas {
public:

  simulation_canvas(int width, int height)
      : buffer(width * height, {0, 0, 0, 0}), width_(width),
        height_(height), elements_(element::get_element_classes()) {}


  void add_particle(int x, int y, element_id_type id) {
    assert(in_canvas(x, y));
    assert(id < elements_.size());

    auto xMin = std::clamp(x - 5, 0, width_);
    auto xMax = std::clamp(x + 5, 0, width_);

    auto yMin = std::clamp(y - 5, 0, height_ - 1);
    auto yMax = std::clamp(y + 5, 0, height_ - 1);

    for (int i = xMin; i < xMax; ++i) {
      for (int j = yMin; j < yMax; ++j) {
        auto &e = get_particle(i, j);
        if (id != 0 && e.id == 0) {
          e = elements_[id].create();
        }
      }
    }
  }

  void delete_particle(int x, int y) {
    assert(in_canvas(x, y));
    auto &e = get_particle(x, y);
    e = elements_[0].create();
  }

  void clear() {
    std::fill(buffer.begin(), buffer.end(), particle_instance{});
  }

  void step_forward() {
    evenFrame ^= 1;
    int xStart = evenFrame ? 0 : width_ - 1;
    int xEnd = evenFrame ? width_ : -1;
    int xInc = evenFrame ? 1 : -1;

    for (int y = height_ - 1; y >= 0; --y) {
      for (int x = xStart; x != xEnd; x += xInc) {
        auto &p = get_particle(x, y);
        assert(p.id < elements_.size());
        if (p.id != 0 && p.lastUpdated != evenFrame) {
          p.lastUpdated = evenFrame;
          elements_[p.id].update(*this, x, y);
        }
      }
    }
  }

  void load();

  void save() const;

  [[nodiscard]] std::size_t size() const { return width_ * height_; }

  void write_to_img(image &img) {
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

  [[nodiscard]] particle_instance &get_particle(int x, int y) {
    assert(in_canvas(x, y));
    return buffer[y * width_ + x];
  }

  [[nodiscard]] const particle_instance &get_particle(int x, int y) const {
    assert(in_canvas(x, y));
    return buffer.at(y * width_ + x);
  }

  [[nodiscard]] bool in_canvas(int x, int y) const {
    return x < width_ && y < height_ && y >= 0 && x >= 0;
  }

  [[nodiscard]] bool on_floor(int y) const {
    return y == height_ - 1;
  }

private:
  std::vector<particle_instance> buffer;
  std::vector<element> elements_;
  int width_, height_;
  bool evenFrame{true};
};

#endif //CPP_FALLING_SAND_SIMULATION_CANVAS_HPP
