#include <simulation/brushes.hpp>
#include <simulation/simulation_canvas.hpp>

void rectangle_brush::operator()(simulation_canvas &canvas, int x, int y,
                                 element_id_type id) const {
  auto xMin = std::clamp(x - size_, 0, canvas.width());
  auto xMax = std::clamp(x + size_, 0, canvas.width());

  auto yMin = std::clamp(y - size_, 0, canvas.height());
  auto yMax = std::clamp(y + size_, 0, canvas.height());
  auto &em = element_manager::instance();

  for (int i = xMin; i < xMax; ++i) {
    for (int j = yMin; j < yMax; ++j) {
      auto &e = canvas.get_particle(i, j);
      if (overwrite_ || e.id == 0 && e.id != id) {
        e = em.get_element(id).create();
      }
    }
  }
}

void rectangle_brush::draw_outline(image &image, int scale) {
  for (int i = 0; i < 2 * size_ * scale; ++i) {
    for (int j = 0; j < scale; ++j) {
      std::fill_n(&image(i, j), 4, 255);
      std::fill_n(&image(j, i), 4, 255);
      std::fill_n(&image(i, 2 * size_ * scale - 1 - j), 4, 255);
      std::fill_n(&image(2 * size_ * scale - 1 - j, i), 4, 255);
    }
  }
}
