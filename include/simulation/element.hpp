#ifndef CPP_FALLING_SAND_ELEMENT_HPP
#define CPP_FALLING_SAND_ELEMENT_HPP

#include <functional>
#include <utility>
#include <simulation/particle.hpp>
#include <string>
#include <random>

class simulation_canvas;

class element_manager;

enum class movement_type {
  powder, solid, liquid, gas
};


class element {
public:
  using update_function = std::function<void(simulation_canvas &canvas,
                                             int x, int y)>;

  element(const element &) = default;

  element(element &&) = default;

  element(movement_type movement, uint8_t red, uint8_t green, uint8_t blue,
          element::update_function update, std::string name_);

  element(movement_type movement, uint8_t red, uint8_t green, uint8_t blue,
          float colorVariation,
          element::update_function update, std::string name_);

  [[nodiscard]] particle_instance create() const;

  element_id_type type{};
  update_function update;
  std::string name;
  std::uint8_t red, green, blue;
  movement_type movement_;
  const bool randomizeColor;
  const float colorVar{};

private:
  void randomize(particle_instance &p) const;
};

#endif //CPP_FALLING_SAND_ELEMENT_HPP
