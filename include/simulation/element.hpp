#ifndef CPP_FALLING_SAND_ELEMENT_HPP
#define CPP_FALLING_SAND_ELEMENT_HPP

#include <functional>
#include <utility>
#include <simulation/particle.hpp>
#include <string>

class simulation_canvas;

class element {
public:
  using update_function = std::function<void(simulation_canvas &canvas,
                                             int x, int y)>;
  using create_function = std::function<particle_instance()>;

  element(const element &) = default;

  element(element &&) = default;

  void static reset_idx() { idx_ = 0; }

  static std::vector<element> &get_element_classes();

  static int get_idx_of(const std::string &name);

  static int empty_id() { return none_idx_; }

  element_id_type type;
  create_function create;
  update_function update;
  std::string name;
private:
  element(create_function create, update_function update, std::string name_);

  static inline std::size_t idx_ = 0;
  static inline std::size_t none_idx_ = 0;
  static inline std::vector<element> allElements_{};
};

#endif //CPP_FALLING_SAND_ELEMENT_HPP
