//
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
    auto &e = get_particle(x, y);
    if (id != 0 && e.id == 0) {
      assert(id < elements_.size());
      e = elements_[id].create();
    }
  }

  void delete_particle(int x, int y) {
    assert(in_canvas(x, y));
    auto &e = get_particle(x, y);
    e = elements_[0].create();
  }

  void step_forward() {
    for (int y = height_ - 1; y >= 0; --y) {
      for (int x = 0; x < width_; ++x) {
        auto &p = get_particle(x, y);
        assert(p.id < elements_.size());
        if (p.id != 0) {
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
    return buffer.at(y * width_ + x);
  }

  [[nodiscard]] const particle_instance &get_particle(int x, int y) const {
    assert(in_canvas(x, y));
    return buffer.at(y * width_ + x);
  }

  [[nodiscard]] bool in_canvas(int x, int y) const {
    return x < width_ && y < height_ && y >= 0 && x >= 0;
  }

private:
  std::vector<particle_instance> buffer;
  std::vector<element> elements_;
  int width_, height_;
  std::uniform_int_distribution<std::uint8_t> red_color{200, 255};
  std::random_device rd;
};

#endif //CPP_FALLING_SAND_SIMULATION_CANVAS_HPP
