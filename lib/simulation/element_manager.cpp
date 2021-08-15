#include <simulation/element_manager.hpp>
#include <simulation/element_update.hpp>

element_manager &element_manager::instance() {
  static element_manager instance{
      {"empty",    movement_type::solid,  0,   0,   0,   0,    {0,   0,   {}},      {}},
      {"boundary", movement_type::solid,  0,   0,   0,   0,    {0,   0,   {}},      {}},
      {"sand",     movement_type::powder, 194, 178, 128, 0.2,  {0,   0,   {}},      {}},
      {"oil",      movement_type::liquid, 120, 103, 33,  0.01, {0,   0,   {}},      {}},
      {"wall",     movement_type::solid,  77,  77,  77,  0,    {0,   0,   {}},      {}},
      {"lava",     movement_type::liquid, 170, 68,  0,   0.5,  {0,   0,   {}},      {
                                                                                        {"water", "steam", 0.8},
                                                                                        {"wood",  "fire",  0.05},
                                                                                        {"oil", "fire", 0.75},
                                                                                    }},
      {"steam",    movement_type::gas,    42,  127, 255, 0.1,  {200, 100, "water"}, {
                                                                                        {"fire",  "empty", 0.15},

                                                                                    }},
      {"water",    movement_type::liquid, 50,  50,  200, 0.2,  {0,   0,   {}},      {
                                                                                        {"fire",  "empty", 0.75},
                                                                                        {"lava",  "sand",  0.1},
                                                                                    }},
      {"wood",     movement_type::solid,  120, 70,  30,  0.1,  {0,   0,   {}},      {
                                                                                        {"water", "wood",  0.01},
                                                                                    }},
      {"fire",     movement_type::gas,    255, 127, 42,  0.5,  {30,  30,  "empty"}, {
                                                                                        {"wood",  "fire",  0.1},
                                                                                        {"water", "steam", 0.5},
                                                                                        {"oil", "fire", 0.5},
                                                                                        {"sand", "lava", 0.005},
                                                                                    }},
  };
  return instance;
}

element_manager::element_manager(const std::initializer_list<element_initializer> &init) :
    contactRules_(init.size() * init.size()), lifetimeRules_(init.size()) {

  allElements_.reserve(init.size());
  element_id_type idx = 0;
  for (const auto &elem:init) {
    allElements_.emplace_back(elem, idx);
    ++idx;
  }

  assert(get_idx("empty") == 0);
  boarderId_ = get_idx("boundary");

  // A bit inefficient, but it only happens once
  for (const auto &elem:init) {
    const auto &lifetime_rule_init = elem.lifetime_rule;
    const auto &contact_rules = elem.contact_rules;
    auto element_id = get_idx(elem.name);
    // A min_lifetime of zero means no lifetime rule is in effect
    if (lifetime_rule_init.min_lifetime != 0) {
      auto new_element_id = get_idx(lifetime_rule_init.new_type_name);
      auto &life_rule = lifetimeRules_[element_id];
      life_rule.min_lifetime = lifetime_rule_init.min_lifetime;
      assert(lifetime_rule_init.transition_period > 1.0f);
      life_rule.transition_prob = 1 / (float) lifetime_rule_init.transition_period;
      life_rule.new_element = new_element_id;
    }

    if (contact_rules.size() > 0) {
      for (auto &rule:contact_rules) {
        auto other_element_id = get_idx(rule.other_element_name);
        auto transform_element_id = get_idx(rule.transform_element_name);
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
  auto water_id = get_idx("water"), oil_id = get_idx("oil");
  displacementRules_[water_id][oil_id] = true;
}

std::size_t element_manager::get_idx(const std::string &name) const {
  auto it = std::find_if(allElements_.begin(), allElements_.end(),
                         [&name](const auto &e) { return e.name == name; });
  assert(it != allElements_.end());
  return it - allElements_.begin();
}

const element &element_manager::get_element(std::size_t idx) const {
  assert(idx < allElements_.size());
  return allElements_[idx];
}
