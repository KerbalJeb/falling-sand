#include <simulation/element.hpp>
#include <simulation/rn_generator.hpp>

particle_instance element::create() const {
  auto p = particle_instance{type, 0, 0, 0, red, green, blue};
  if (randomize_color) { randomize(p); }
  return p;
}

void element::randomize(particle_instance &p) const {
  auto scale = 1 - color_var * rng::instance().rand_float();
  p.r = static_cast<std::uint8_t>(scale * p.r);
  p.g = static_cast<std::uint8_t>(scale * p.g);
  p.b = static_cast<std::uint8_t>(scale * p.b);
}

element::element(const element_initializer &init, element_id_type id) {
  type = id;
  name = init.name;
  red = init.red;
  green = init.green;
  blue = init.blue;
  movement = init.movement;
  randomize_color = init.color_variation > 0;
  color_var = init.color_variation;
  opt_param = init.opt_param;
}
