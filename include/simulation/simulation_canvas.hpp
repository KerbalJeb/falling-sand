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
#include <simulation/rn_generator.hpp>
#include <graphics/image.hpp>

// Class responsible for updating the falling sand simulation
class simulation_canvas {
public:

  using brush = std::function<void(simulation_canvas &, int, int, int)>;

  // Create an empty canvas of the given size
  // The element manager contains a list of elements to be simulated on the canvas
  simulation_canvas(int width, int height, element_manager em);

  // The simulation canvas should not need to be moved or copied
  simulation_canvas(const simulation_canvas &) = delete;

  simulation_canvas(simulation_canvas &&) = delete;

  // Add particles of the given id using a brush
  void add_particle(int x, int y, element_id_type id, const brush &b);

  // Add a single particle of the given id at the given position
  void add_particle(int x, int y, element_id_type id);

  // Clear all elements from the canvas
  void clear();

  // Advance the simulation by one time step
  void step_forward();

  // Load a save file from the given path
  bool load(const std::filesystem::path &path);

  // Save the current state of the canvas to the given path
  bool save(const std::filesystem::path &path) const;

  // Save the current state of the canvas using the default name
  // falling-sand-n.sav, where n is the smallest integer such there are no existing files with the name
  bool save() const;

  // Write the current canvas state to an image
  void write_to_image(image &img);

  // Swap two particles positions
  void swap_particles(particle_instance &p1, particle_instance &p2) {
    std::swap(p1, p2);
  }

  // Get the partile at (x,y)
  [[nodiscard]] particle_instance &get_particle(int x, int y);

  [[nodiscard]] const particle_instance &get_particle(int x, int y) const;

  [[nodiscard]] bool in_canvas(int x, int y) const;

  // Width of the canvas
  [[nodiscard]] int width() const { return width_ - 2; }

  // Height of the canvas
  [[nodiscard]] int height() const { return height_ - 2; }

  // Max number of particles in the canvas
  [[nodiscard]] std::size_t size() const { return width() * height(); }

  // Get the canvas's element manager
  [[nodiscard]] const element_manager &get_element_manager() const { return elementManager_; }

private:
  constexpr static std::int16_t gravity_accel = 2;

  [[nodiscard]] bool in_bounds(int x, int y) const;

  void fill_boundary();

  bool move_powder(int x, int y, particle_instance &p, const element &e);

  void move_liquid(int x, int y);

  void move_gas(int x, int y);

  std::vector<particle_instance> buffer;
  int width_, height_;
  bool evenFrame{true};
  element_manager elementManager_;
  rng &rn_gen = rng::instance();

  [[nodiscard]] std::string create_save_file_name(int save_idx) const;

  const std::string save_file_name_{"falling-sand"};
  const std::string save_file_ext_{"sav"};
};

#endif //CPP_FALLING_SAND_SIMULATION_CANVAS_HPP
