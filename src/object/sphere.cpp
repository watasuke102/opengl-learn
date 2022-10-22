#include "object/sphere.hpp"

#include <iostream>
#include <vector>

namespace {
constexpr GLfloat  SPHERE_RADIUS = 1.f;
constexpr uint32_t SPHERE_DIV    = 32;
}  // namespace

namespace gl_learn {
Sphere::Sphere(glm::vec3 pos, glm::quat quat) {
  this->pos  = pos;
  this->quat = quat;
  this->init();
};

void Sphere::init() {
  this->vertex.clear();
  this->elements.clear();

  constexpr auto div_theta = glm::pi<float>() / SPHERE_DIV;
  for (auto i = 0u; i <= SPHERE_DIV; ++i) {
    const float y = cosf(div_theta * i);
    const float r = SPHERE_RADIUS * sinf(acosf(y / SPHERE_RADIUS));
    for (auto j = 0u; j < SPHERE_DIV; ++j) {
      const float x = r * cosf(2 * div_theta * j);
      const float z = r * sinf(2 * div_theta * j);
      this->vertex.push_back({x, y, z});

      // horizontal lines
      if (i != 0 && i != SPHERE_DIV) {
        const GLuint index_y = i * SPHERE_DIV + j;
        if (j == SPHERE_DIV - 1) {
          this->elements.push_back({index_y, i * SPHERE_DIV});
        } else {
          this->elements.push_back({index_y, index_y + 1});
        }
      }
      // vertical line
      if (i != 0 && j != 0) {
        const GLuint index_xz = SPHERE_DIV * j - i;
        this->elements.push_back({index_xz, index_xz + SPHERE_DIV});
      }
    }
  }
  // vertical (bottom)
  for (GLuint i = 1; i <= SPHERE_DIV; ++i) {
    this->elements.push_back(
        {SPHERE_DIV * SPHERE_DIV - i, SPHERE_DIV * SPHERE_DIV}
    );
  }

  this->set_buffer_data();
}

void Sphere::animate() {
  // this->quat *= glm::quat(glm::vec3(0.f, 0.05f, 0.f));
}
}  // namespace gl_learn
