#include <graphics/gl/vertex_array.hpp>

vertex_buffer_layout::vertex_buffer_layout(const vertex_attribute &attribute) : layout_{} {
  layout_.push_back(attribute);
  stride_ = 0;
}

vertex_buffer_layout::vertex_buffer_layout(std::initializer_list<vertex_attribute> attributes)
    : layout_(attributes) {
  stride_ = std::accumulate(layout_.begin(), layout_.end(), 0,
                            [](int a, const vertex_attribute &b) {
                              return a +
                                     vertex_attribute::size_mapping[b.type] *
                                     b.count;
                            });
}

void vertex_buffer_layout::apply_layout() const {
  std::size_t offset = 0;
  for (int i = 0; i < layout_.size(); ++i) {
    const auto &attrib = layout_[i];
    int element_size = vertex_attribute::size_mapping[attrib.type];
    glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalize,
                          stride_, (void *) offset);
    glEnableVertexAttribArray(i);
    offset += element_size * attrib.count;
  }
}

vertex_array::vertex_array() {
  glGenVertexArrays(1, &obj_);
  assert(obj_ != 0);
}

vertex_array::vertex_array(const vertex_buffer &buffer, const vertex_buffer_layout &layout)
    : vertex_array() {
  add_buffer(buffer, layout);
}

vertex_array &vertex_array::operator=(vertex_array &&other) noexcept {
  if (this != &other) {
    release();
    std::swap(obj_, other.obj_);
  }
  return *this;
}

void vertex_array::add_buffer(const vertex_buffer &buffer, const vertex_buffer_layout &layout) const {
  bind();
  buffer.bind();
  layout.apply_layout();
}
