//
// Created by ben on 2021-07-20.
//

#ifndef CPP_FALLING_SAND_ELEMENT_MANAGER_HPP
#define CPP_FALLING_SAND_ELEMENT_MANAGER_HPP

#include <vector>
#include <simulation/element.hpp>
#include <initializer_list>

class element_manager {
public:
  element_manager() = delete;

  element_manager(const element_manager &) = delete;

  element_manager(element_manager &&) = delete;

  static element_manager &instance();

  [[nodiscard]] const std::vector<element> &
  elements() const { return allElements_; }

  [[nodiscard]] std::size_t get_idx(const std::string &name) const;

  [[nodiscard]] std::size_t size() const;

  [[nodiscard]] const element &get_element(std::size_t idx) const;

private:
  element_manager(std::initializer_list<element> init);

  std::vector<element> allElements_;
};

#endif //CPP_FALLING_SAND_ELEMENT_MANAGER_HPP
