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

  using brush = std::function<void(simulation_canvas &, int, int, int)>;

  struct brush_base {
    brush_base(int radius, bool overwrite)
        : size_(radius), overwrite_(overwrite) {}

    virtual void operator()(simulation_canvas &canvas, int x, int y,
                            element_id_type id) const = 0;

    void set_size(int s) { size_ = s; }

    void set_overwrite(bool o) { overwrite_ = o; }

    [[nodiscard]] int size() const { return size_; }

  protected:
    int size_;
    bool overwrite_;
  };

  struct rectangle_brush : public brush_base {
    using brush_base::brush_base;

    void operator()(simulation_canvas &canvas, int x, int y,
                    element_id_type id) const override {
      auto xMin = std::clamp(x - size_, 0, canvas.width_);
      auto xMax = std::clamp(x + size_, 0, canvas.width_);

      auto yMin = std::clamp(y - size_, 0, canvas.height_ - 1);
      auto yMax = std::clamp(y + size_, 0, canvas.height_ - 1);

      for (int i = xMin; i < xMax; ++i) {
        for (int j = yMin; j < yMax; ++j) {
          auto &e = canvas.get_particle(i, j);
          if (overwrite_ || e.id == 0) {
            e = canvas.elements_[id].create();
          }
        }
      }
    }

    void draw_outline(image &image, int scale = 1) {
      for (int i = 0; i < 2 * size_ * scale; ++i) {
        for (int j = 0; j < scale; ++j) {
          std::fill_n(&image(i, j), 4, 255);
          std::fill_n(&image(j, i), 4, 255);
          std::fill_n(&image(i, 2 * size_ * scale - 1 - j), 4, 255);
          std::fill_n(&image(2 * size_ * scale - 1 - j, i), 4, 255);
        }
      }
    }
  };

  simulation_canvas(int width, int height)
      : buffer(width * height, {0, 0, 0, 0}), width_(width),
        height_(height), elements_(element::get_element_classes()) {}

  simulation_canvas(const simulation_canvas &) = delete;

  simulation_canvas(simulation_canvas &&) = delete;

  void add_particle(int x, int y, element_id_type id, const brush &b) {
    assert(in_canvas(x, y));
    assert(id < elements_.size());
    b(*this, x, y, id);
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
  friend brush_base;
  std::vector<particle_instance> buffer;
  std::vector<element> elements_;
  int width_, height_;
  bool evenFrame{true};
};

#endif //CPP_FALLING_SAND_SIMULATION_CANVAS_HPP
