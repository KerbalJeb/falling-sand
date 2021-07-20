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
#include <simulation/element_manager.hpp>
#include <graphics/image.hpp>

class simulation_canvas {
public:

  using brush = std::function<void(simulation_canvas &, int, int, int)>;

  simulation_canvas(int width, int height);

  simulation_canvas(const simulation_canvas &) = delete;

  simulation_canvas(simulation_canvas &&) = delete;

  void add_particle(int x, int y, element_id_type id, const brush &b);

  void clear();

  void step_forward();

  void load();

  void save() const;

  [[nodiscard]] std::size_t size() const { return width_ * height_; }

  void write_to_img(image &img);

  [[nodiscard]] particle_instance &get_particle(int x, int y);

  [[nodiscard]] const particle_instance &get_particle(int x, int y) const;

  [[nodiscard]] bool in_canvas(int x, int y) const;

  [[nodiscard]] bool on_floor(int y) const;

  [[nodiscard]] int width() const { return width_; }

  [[nodiscard]] int height() const { return height_; }

private:
  std::vector<particle_instance> buffer;
  int width_, height_;
  bool evenFrame{true};
};

#endif //CPP_FALLING_SAND_SIMULATION_CANVAS_HPP
