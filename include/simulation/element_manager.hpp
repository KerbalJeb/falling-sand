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

  // Creates an element manager with the given elements
  // First entry must be the empty element
  // The list must also contain one solid element with the name 'boundary'
  element_manager(const std::initializer_list<element_initializer> &elements);

  element_manager(const element_manager &) = delete;

  element_manager(element_manager &&) = default;

  // Get the id of an element from its string name
  [[nodiscard]] std::size_t find_id(const std::string &name) const;

  // The total number of elements
  [[nodiscard]] std::size_t size() const { return allElements_.size(); }

  // The id of the empty element
  [[nodiscard]] std::size_t empty_id() const { return 0; }

  // The id of the boundary element
  [[nodiscard]] std::size_t boundary_id() const { return boarderId_; }

  // Get the element with the given id
  [[nodiscard]] const element &get_element(std::size_t id) const;

  // Check if primary can displays secondary
  [[nodiscard]] bool
  get_displacement_rule(element_id_type primary,
                        element_id_type secondary) const { return displacementRules_[primary][secondary]; }

  // Get the life time rule for p
  [[nodiscard]] const lifetime_rule &
  get_lifetime_rule(element_id_type p) const { return lifetimeRules_[p]; }

  // Get the contact rules for primary and secondary
  [[nodiscard]] const contact_rule &
  get_contact_rule(element_id_type primary, element_id_type secondary) const {
    return contactRules_[primary * size() + secondary];
  }

private:
  std::vector<element> allElements_;
  std::vector<std::vector<bool>> displacementRules_;
  std::vector<contact_rule> contactRules_;
  std::vector<lifetime_rule> lifetimeRules_;
  element_id_type boarderId_;
};

#endif //CPP_FALLING_SAND_ELEMENT_MANAGER_HPP
