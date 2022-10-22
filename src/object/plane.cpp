#include "object/plane.hpp"
#include <iostream>
#include <vector>

namespace gl_learn {
Plane::Plane(glm::vec3 pos, glm::quat quat) {
  this->pos  = pos;
  this->quat = quat;
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

void Plane::animate() {
  this->quat *= glm::quat(glm::vec3(0.f, -0.05f, 0.f));
}
} // namespace gl_learn
