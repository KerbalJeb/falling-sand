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

// A struct used to initialize elements
// Needed because elements can only be fully constructed by the element manager
// since element integer ids are generated at runtime (by the element manager)
struct element_initializer {
  struct lifetime {
    // Minimum lifetime of the element
    // If 0, element has infinite lifetime
    int min_lifetime{0};
    // The average time to death after min_lifetime has elapsed
    float transition_period{0};
    // The element to turn into on death
    std::string new_type_name{};
  };

  struct contact_rule {
    std::string other_element_name{0};
    std::string transform_element_name{0};
    float transform_chance{0};
  };

  // Name of the element
  std::string name;
  // Element movement rule
  movement_type movement;
  // Element color
  std::uint8_t red, green, blue;
  // A value between 0-1 that indicates how much to randomize the element color
  // With 0 being no variation and 1 being max variation
  float color_variation;
  lifetime lifetime_rule{};
  std::initializer_list<contact_rule> contact_rules{};
};

// A class the represents one element type
// Stores all element properties and is responsible for creating
// new instances (particles) of its element type
class element {
public:
  element(const element &) = default;

  element(element &&) = default;

  // Create an element from the given element_initializer with the given integer id
  element(const element_initializer &init, element_id_type id);

  // Create a new particle of this element type
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
