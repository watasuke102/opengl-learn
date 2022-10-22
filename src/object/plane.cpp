#include "object/plane.hpp"
#include <vector>

namespace gl_learn {
void Plane::init() {
  this->vertex.clear();
  this->vertex.assign({
      {0.5f,  0.5f,  0.1f},
      {0.5f,  -0.5f, 1.f },
      {-0.5f, -0.5f, 1.f },
      {-0.5f, 0.5f,  0.1f},
  });

  this->elements.clear();
  this->elements.assign({
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  });

  this->set_buffer_data();
}
} // namespace gl_learn
