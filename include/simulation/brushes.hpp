//
// Created by ben on 2021-07-20.
//

#ifndef CPP_FALLING_SAND_BRUSHES_HPP
#define CPP_FALLING_SAND_BRUSHES_HPP

#include <graphics/image.hpp>
#include <simulation/simulation_canvas.hpp>

struct brush_base {
  brush_base(int radius, bool overwrite, int maxSize = 40, int minSize = 1)
      : size_(radius),
        overwrite_(overwrite),
        maxSize_(maxSize),
        minSize_(minSize) {}

  virtual void operator()(simulation_canvas &canvas, int x, int y,
                          element_id_type id) const = 0;

  void set_size(int s) { size_ = std::clamp(s, minSize_, maxSize_); }

  void set_overwrite(bool o) { overwrite_ = o; }

  [[nodiscard]] int size() const { return size_; }

protected:
  int size_, maxSize_, minSize_;
  bool overwrite_;
};

struct rectangle_brush : public brush_base {
  using brush_base::brush_base;

  void operator()(simulation_canvas &canvas, int x, int y,
                  element_id_type id) const override;

  void draw_outline(image &image, int scale = 1);
};

#endif //CPP_FALLING_SAND_BRUSHES_HPP
