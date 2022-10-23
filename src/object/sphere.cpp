#include "object/sphere.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

namespace {
constexpr GLfloat  SPHERE_RADIUS = 1.f;
constexpr uint32_t SPHERE_DIV    = 32;
}  // namespace

namespace gl_learn {
Sphere::Sphere(glm::vec3 pos, glm::quat quat) : Object(GL_LINES) {
  this->pos  = pos;
  this->quat = quat;
  this->init();
};

void Sphere::init() {
  this->vertex.clear();
  this->elements.clear();

  constexpr auto div_theta = glm::pi<float>() / SPHERE_DIV;
  for (auto i = 0u; i <= SPHERE_DIV; ++i) {
    const float y = SPHERE_RADIUS * cosf(div_theta * i);
    const float r = SPHERE_RADIUS * sinf(acosf(y / SPHERE_RADIUS));
    for (auto j = 0u; j < SPHERE_DIV; ++j) {
      if ((i != 0 && i != SPHERE_DIV) || j == 0) {
        const float x = r * cosf(2 * div_theta * j);
        const float z = r * sinf(2 * div_theta * j);
        this->vertex.push_back({x, y, z});
      }

      // vertical line
      if (i != SPHERE_DIV) {
        const GLint index_xz = (i - 1) * (SPHERE_DIV - 1) + i + j;
        this->elements.push_back((i != 0) * index_xz);
        this->elements.push_back(
            (i == SPHERE_DIV - 1) ? (SPHERE_DIV * (SPHERE_DIV - 1) + 1) :
                                    (index_xz + SPHERE_DIV)
        );
        // std::cout << "[" << i << ", " << j << ", " << std::setw(2) <<
        // index_xz
        //           << "] " << std::setw(2) << *(this->elements.end() - 2) <<
        //           ", "
        //           << std::setw(2) << *(this->elements.end() - 1) <<
        //           std::endl;
      }

      // horizontal lines
      if (i != 0 && i != SPHERE_DIV) {
        const GLuint index_y = (i - 1) * SPHERE_DIV + j + 1;
        if (j == SPHERE_DIV - 1) {
          this->elements.push_back(index_y);
          this->elements.push_back((i - 1) * SPHERE_DIV + 1);
        } else {
          this->elements.push_back(index_y);
          this->elements.push_back(index_y + 1);
        }
      }
    }
  }

  this->set_buffer_data();
}

void Sphere::animate() {
  this->quat *= glm::quat(glm::vec3(0.00f, 0.01f, 0.f));
}
}  // namespace gl_learn
