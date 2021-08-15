//
// Created by ben on 2021-07-20.
//

#ifndef CPP_FALLING_SAND_BRUSHES_HPP
#define CPP_FALLING_SAND_BRUSHES_HPP

#include <graphics/image.hpp>
#include <simulation/simulation_canvas.hpp>

// An abstract base class for all brushes used to draw on canvas
struct brush_base {
  // Takes the initial brush radius in px,
  // the max and min bush sizes and weather to override/replace exising elements
  // A radius of zero will result in a brush size of 1 px
  brush_base(int radius, bool overwrite, int maxRad = 40, int minRad = 0)
      : overwrite_(overwrite),
        maxSize_(maxRad),
        minSize_(minRad) { set_radius(radius); }

  // Draw with the brush on a canvas at (x,y) using the provided element id
  virtual void operator()(simulation_canvas &canvas, int x, int y,
                          element_id_type id) const = 0;

  // Draw the outline of the brush on an image, scaled by the scale factor
  virtual void draw_outline(image &image, int scale) = 0;

  // Set the radius of the brush in px
  void set_radius(int s) { size_ = std::clamp(s, minSize_, maxSize_); }

  // Set if the brush can replace exising elements on the canvas
  void set_overwrite(bool o) { overwrite_ = o; }

  // Get the size of the brush in px
  [[nodiscard]] int size() const { return size_; }


protected:
  int size_, maxSize_, minSize_;
  bool overwrite_;
};

// The rectangular brush
struct rectangle_brush : public brush_base {
  // Use parent constructor
  using brush_base::brush_base;

  void operator()(simulation_canvas &canvas, int x, int y,
                  element_id_type id) const override;

  void draw_outline(image &image, int scale = 1) override;
};

#endif //CPP_FALLING_SAND_BRUSHES_HPP
