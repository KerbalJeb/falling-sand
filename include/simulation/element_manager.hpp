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

  struct lifetime_rule {
    std::uint16_t min_lifetime{0};
    float transition_prob{0};
    element_id_type new_element{0};
  };

  struct contact_rule {
    float transform_chance{0};
    element_id_type transform_element{0};
  };

  element_manager() = delete;

  element_manager(const element_manager &) = delete;

  element_manager(element_manager &&) = delete;

  static element_manager &instance();

  [[nodiscard]] const std::vector<element> &
  elements() const { return allElements_; }

  [[nodiscard]] std::size_t get_idx(const std::string &name) const;

  [[nodiscard]] std::size_t size() const;

  [[nodiscard]] const element &get_element(std::size_t idx) const;

  [[nodiscard]] bool
  get_displacement_rule(element_id_type p, element_id_type s) const { return displacementRules_[p][s]; }

  [[nodiscard]] const lifetime_rule &
  get_lifetime_rule(element_id_type p) const { return lifetimeRules_[p]; }

  [[nodiscard]] const contact_rule &
  get_contact_rule(element_id_type p, element_id_type s) const { return contactRules_[p * size() + s]; }

private:
  element_manager(const std::initializer_list<element_initializer> &init);

  std::vector<element> allElements_;
  std::vector<std::vector<bool>> displacementRules_;
  std::vector<contact_rule> contactRules_;
  std::vector<lifetime_rule> lifetimeRules_;
};

#endif //CPP_FALLING_SAND_ELEMENT_MANAGER_HPP
