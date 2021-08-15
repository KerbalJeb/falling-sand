#include <simulation/element_manager.hpp>
#include <cassert>

element_manager::element_manager(const std::initializer_list<element_initializer> &elements) :
    contactRules_(elements.size() * elements.size()), lifetimeRules_(elements.size()) {

  allElements_.reserve(elements.size());
  element_id_type idx = 0;
  for (const auto &elem:elements) {
    allElements_.emplace_back(elem, idx);
    ++idx;
  }

  assert(find_id("empty_id") == 0);
  boarderId_ = find_id("boundary");

  // A bit inefficient, but it only happens once
  for (const auto &elem:elements) {
    const auto &lifetime_rule_init = elem.lifetime_rule;
    const auto &contact_rules = elem.contact_rules;
    auto element_id = find_id(elem.name);
    // A min_lifetime of zero means no lifetime rule is in effect
    if (lifetime_rule_init.min_lifetime != 0) {
      auto new_element_id = find_id(lifetime_rule_init.new_type_name);
      auto &life_rule = lifetimeRules_[element_id];
      life_rule.min_lifetime = lifetime_rule_init.min_lifetime;
      assert(lifetime_rule_init.transition_period > 1.0f);
      life_rule.transition_prob = 1 / (float) lifetime_rule_init.transition_period;
      life_rule.new_element = new_element_id;
    }

    if (contact_rules.size() > 0) {
      for (auto &rule:contact_rules) {
        auto other_element_id = find_id(rule.other_element_name);
        auto transform_element_id = find_id(rule.transform_element_name);
        contactRules_[element_id * size() + other_element_id].transform_element = transform_element_id;
        contactRules_[element_id * size() + other_element_id].transform_chance = rule.transform_chance;
      }
    }
  }

  std::size_t n = allElements_.size();
  for (int i = 0; i < n; ++i) {
    displacementRules_.emplace_back(size(), false);
    for (int j = 0; j < n; ++j) {
      auto &e1 = get_element(i);
      auto &e2 = get_element(j);
      if (e2.type == 0) {
        displacementRules_[i][j] = true;
      }
      if (e1.movement == movement_type::powder &&
          (e2.movement == movement_type::liquid || e2.movement == movement_type::gas)) {
        displacementRules_[i][j] = true;
      }
      if (e1.movement == movement_type::liquid && e2.movement == movement_type::gas) {
        displacementRules_[i][j] = true;
      }
    }
  }

  // Just hard coding this for now, could add a density property later
  auto water_id = find_id("water"), oil_id = find_id("oil");
  displacementRules_[water_id][oil_id] = true;
}

std::size_t element_manager::find_id(const std::string &name) const {
  auto it = std::find_if(allElements_.begin(), allElements_.end(),
                         [&name](const auto &e) { return e.name == name; });
  assert(it != allElements_.end());
  return it - allElements_.begin();
}

const element &element_manager::get_element(std::size_t id) const {
  assert(id < allElements_.size());
  return allElements_[id];
}
