#include "object/plane.hpp"
#include <iostream>
#include <vector>

namespace gl_learn {
Plane::Plane(float x, float y, float z) {
  this->pos = glm::vec3(x, y, z);
  this->init();
};

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
