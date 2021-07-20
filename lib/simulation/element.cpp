#include <simulation/element.hpp>
#include <simulation/element_update.hpp>
#include <simulation/rn_generator.hpp>
#include <utility>


particle_instance element::create() const {
  auto p = particle_instance{type, 0, 0, 0, red_, green_, blue_};
  if (randomizeColor) { randomize(p); }
  return p;
}

void element::randomize(particle_instance &p) const {
  auto scale = 1 - colorVar_ * rng::instance().rand_float();
  p.r = static_cast<std::uint8_t>(scale * p.r);
  p.g = static_cast<std::uint8_t>(scale * p.g);
  p.b = static_cast<std::uint8_t>(scale * p.b);
}

element::element(uint8_t red, uint8_t green, uint8_t blue,
                 element::update_function update, std::string name_)
    : update(std::move(update)),
      red_{red}, green_{green}, blue_{blue},
      name(std::move(name_)),
      randomizeColor{false} {

}

element::element(uint8_t red, uint8_t green, uint8_t blue, float colorVariation,
                 element::update_function update, std::string name_)
    : update(std::move(update)),
      name(std::move(name_)),
      red_{red}, green_{green}, blue_{blue},
      randomizeColor{true},
      colorVar_{colorVariation} {

}
