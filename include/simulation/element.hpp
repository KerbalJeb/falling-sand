#ifndef CPP_FALLING_SAND_ELEMENT_HPP
#define CPP_FALLING_SAND_ELEMENT_HPP

#include <functional>
#include <utility>
#include <simulation/particle.hpp>
#include <string>
#include <random>

class simulation_canvas;

class element_manager;


class element {
public:
  using update_function = std::function<void(simulation_canvas &canvas,
                                             int x, int y)>;

  element(const element &) = default;

  element(element &&) = default;

  element(uint8_t red, uint8_t green, uint8_t blue,
          element::update_function update, std::string name_);

  element(uint8_t red, uint8_t green, uint8_t blue, float colorVariation,
          element::update_function update, std::string name_);

  particle_instance create() const;

  element_id_type type{};
  update_function update;
  std::string name;
  std::uint8_t red_, green_, blue_;
  const bool randomizeColor;
  const float colorVar_{};

private:
  void randomize(particle_instance &p) const;
};

#endif //CPP_FALLING_SAND_ELEMENT_HPP
