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


struct element_initializer {
  struct lifetime {
    int min_lifetime{0};
    int transition_period{};
    std::string new_type_name{};
  };

  struct contact_rule {
    float transform_chance{0};
    std::string transform_element_name{0};
    std::string other_element_name{0};
  };

  std::string name;
  movement_type movement;
  std::uint8_t red, green, blue;
  float color_variation;
  lifetime lifetime_rule{};
  std::initializer_list<contact_rule> contact_rules{};
};

class element {
public:
  element(const element &) = default;

  element(element &&) = default;

//  element(movement_type movement, uint8_t red, uint8_t green, uint8_t blue, std::string name_);

//  element(movement_type movement, uint8_t red, uint8_t green, uint8_t blue, float colorVariation, std::string name_);
  element(const element_initializer &init, element_id_type id);

  [[nodiscard]] particle_instance create() const;

  element_id_type type;
  std::string name;
  std::uint8_t red, green, blue;
  movement_type movement;
  bool randomizeColor;
  float colorVar{};

private:
  void randomize(particle_instance &p) const;
};

#endif //CPP_FALLING_SAND_ELEMENT_HPP
