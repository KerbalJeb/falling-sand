#include <simulation/element_manager.hpp>
#include <simulation/element_update.hpp>

element_manager &element_manager::instance() {
  static element_manager instance{
      {movement_type::solid,  0,   0,   0,   nullptr, "empty"},
      {movement_type::solid,  0,   0,   0,   nullptr, "boundary"},
      {movement_type::powder, 194, 178, 128, 0.2f,    nullptr, "sand"},
      {movement_type::liquid, 50,  50,  200, 0.2f,    nullptr, "water"},
      {movement_type::solid,  120, 70,  30,  nullptr, "wood"},
      {movement_type::gas,    255, 127, 42,  nullptr, "fire"},
      {movement_type::liquid, 170, 68,  0,   nullptr, "lava"},
      {movement_type::liquid, 120, 103, 33,  nullptr, "oil"},
      {movement_type::solid,  77,  77,  77,  nullptr, "wall"},
      {movement_type::gas,    42,  127, 255, nullptr, "steam"},
  };
  return instance;
}

const std::vector<element> &element_manager::elements() const {
  return allElements_;
}

element_manager::element_manager(std::initializer_list<element> init)
    : allElements_(init) {
  for (int i = 0; i < allElements_.size(); ++i) {
    allElements_[i].type = i;
  }
}

std::size_t element_manager::get_idx(const std::string &name) const {
  auto it = std::find_if(allElements_.begin(), allElements_.end(),
                         [&name](const auto &e) { return e.name == name; });
  assert(it != allElements_.end());
  return it - allElements_.begin();
}

std::size_t element_manager::size() const {
  return allElements_.size();
}

const element &element_manager::get_element(std::size_t idx) const {
  assert(idx < allElements_.size());
  return allElements_[idx];
}

