#include <simulation/element.hpp>
#include <simulation/elements/elements.hpp>
#include <utility>

std::vector<element> &element::get_element_classes() {
  if (allElements_.empty()) {
    idx_ = 0;
    allElements_ = std::vector<element>{
        {elements::create_empty{}, nullptr,                 "empty"},
        {elements::create_sand{},  elements::update_sand{}, "sand"},
    };
  }
  return allElements_;
}

element::element(element::create_function create,
                 element::update_function update, std::string name_)
    : create(std::move(create)), update(std::move(update)),
      type(idx_), name(std::move(name_)) { ++idx_; }

int element::get_idx_of(const std::string &name) {
  auto &ec = get_element_classes();
  auto it = std::find_if(ec.begin(), ec.end(),
                         [name](const auto &e) { return e.name == name; });
  if (it == ec.end()) { return -1; }
  return it - ec.begin();
}
