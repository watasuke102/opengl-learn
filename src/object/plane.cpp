#include "object/plane.hpp"

#include <iostream>
#include <vector>

namespace gl_learn {
Plane::Plane(glm::vec3 pos, glm::quat quat) : Object(GL_TRIANGLES) {
  this->pos  = pos;
  this->quat = quat;
  this->init();
};

void Plane::init() {
  this->vertex.assign({
      { 0.5f,  0.5f, 0.f},
      { 0.5f, -0.5f, 0.f},
      {-0.5f, -0.5f, 0.f},
      {-0.5f,  0.5f, 0.f},
  });

  this->normal.assign({
      {0, 0, 1},
      {0, 0, 1},
      {0, 0, 1},
      {0, 0, 1}
  });

  this->elements.assign({
      0, 1, 2,  //
      2, 3, 0,  //
  });

  this->uv.assign({
      {1, 0},
      {1, 1},
      {0, 1},
      {0, 0},
  });

  this->load_texture("assets/icon.raw", 512, 512);
  this->set_buffer_data();
}

void Plane::animate() {
  this->quat *= glm::quat(glm::vec3(-0.03f, 0.f, 0.f));
  this->pos.x += 0.005f;
  this->normal.clear();
  for (int i = 0; i < 4; ++i) {
    auto v = glm::normalize(this->quat * glm::vec3(0, 0, 1));
    this->normal.push_back({v.x, v.y, v.z});
  }
  this->set_buffer_data();
}
}  // namespace gl_learn
