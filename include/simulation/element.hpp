#ifndef CPP_FALLING_SAND_ELEMENT_HPP
#define CPP_FALLING_SAND_ELEMENT_HPP

#include <functional>
#include <utility>
#include <simulation/particle.hpp>
#include <string>
#include <random>

class simulation_canvas;


class element {
public:
  struct creation_func {
    using id_type = std::uint8_t;

    creation_func(uint8_t red, uint8_t green, uint8_t blue,
                  float colorVariation)
        : id_(0), red_(red), green_(green), blue_(blue),
          dist_(1 - colorVariation, 1), randomizeColor(true) {}

    creation_func(uint8_t red, uint8_t green, uint8_t blue)
        : id_(0),
          red_(red),
          green_(green),
          blue_(blue),
          randomizeColor(false) {}

    particle_instance operator()() {
      if (randomizeColor) {
        particle_instance particleInstance{id_, 0, 0, 0};
        float colorScale = dist_(rg_);
        particleInstance.r = static_cast<std::uint8_t>(std::clamp(
            colorScale * red_, 0.0f, 255.0f));

        particleInstance.g = static_cast<std::uint8_t>(std::clamp(
            colorScale * green_, 0.0f, 255.0f));

        particleInstance.b = static_cast<std::uint8_t>(std::clamp(
            colorScale * blue_, 0.0f, 255.0f));
        return particleInstance;
      } else {
        return particle_instance{id_, 0, 0, 0, red_, green_, blue_};
      }
    }

    void set_id(id_type id) { id_ = id; }

  private:
    id_type id_, red_, green_, blue_;
    const bool randomizeColor;
    static inline std::random_device rg_{};
    std::uniform_real_distribution<float> dist_;
  };

  using update_function = std::function<void(simulation_canvas &canvas,
                                             int x, int y)>;

  element(const element &) = default;

  element(element &&) = default;

  void static reset_idx() { idx_ = 0; }

  static std::vector<element> &get_element_classes();

  static int get_idx_of(const std::string &name);

  static int empty_id() { return none_idx_; }

  element_id_type type;
  creation_func create;
  update_function update;
  std::string name;
private:
  element(creation_func create, update_function update, std::string name_);

  static inline int idx_ = 0;
  static inline std::size_t none_idx_ = 0;
  static inline std::vector<element> allElements_{};
};

#endif //CPP_FALLING_SAND_ELEMENT_HPP
