#include <simulation/element.hpp>
#include <simulation/element_update.hpp>
#include <simulation/rn_generator.hpp>
#include <utility>


particle_instance element::create() const {
  auto p = particle_instance{type, 0, 0, 0, red, green, blue};
  if (randomizeColor) { randomize(p); }
  return p;
}

void element::randomize(particle_instance &p) const {
  auto scale = 1 - colorVar * rng::instance().rand_float();
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
  randomizeColor = init.color_variation > 0;
  colorVar = init.color_variation;
}
