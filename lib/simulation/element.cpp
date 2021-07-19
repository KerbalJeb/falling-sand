#include <simulation/element.hpp>
#include <simulation/elements/elements.hpp>
#include <utility>

std::vector<element> &element::get_element_classes() {
  if (allElements_.empty()) {
    idx_ = 0;
    allElements_ = std::vector<element>{
        {creation_func{0, 0, 0, 0.0f},       nullptr,                  "empty"},
        {creation_func{194, 178, 128, 0.2f}, elements::update_sand{},  "sand"},
        {creation_func{50, 50, 200, 0.2f},   elements::update_water{}, "water"},
    };
    for (int i = 0; i < allElements_.size(); ++i) {
      allElements_[i].create.set_id(i);
    }
  }
  return allElements_;
}

element::element(creation_func create,
                 element::update_function update, std::string name_)
    : create(create), update(std::move(update)),
      type(idx_), name(std::move(name_)) { ++idx_; }

int element::get_idx_of(const std::string &name) {
  auto &ec = get_element_classes();
  auto it = std::find_if(ec.begin(), ec.end(),
                         [name](const auto &e) { return e.name == name; });
  assert(it != ec.end());
  return it - ec.begin();
}
