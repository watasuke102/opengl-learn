#include "object/sphere.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {
constexpr GLfloat  SPHERE_RADIUS = 1.f;
constexpr uint32_t SPHERE_DIV    = 64;
}  // namespace

namespace gl_learn {
Sphere::Sphere(glm::vec3 pos, glm::quat quat) : Object(GL_TRIANGLES) {
  this->pos  = pos;
  this->quat = quat;
  this->init();
};

void Sphere::init() {
  std::array<std::vector<GLuint>, SPHERE_DIV - 1> points;
  constexpr auto div_theta = glm::pi<float>() / SPHERE_DIV;

  for (auto i = 0u; i <= SPHERE_DIV; ++i) {
    const float y = SPHERE_RADIUS * cosf(div_theta * i);
    const float r = SPHERE_RADIUS * sinf(acosf(y / SPHERE_RADIUS));
    for (auto j = 0u; j <= SPHERE_DIV; ++j) {
      if ((i != 0 && i != SPHERE_DIV) || j == 0) {
        const float x = r * cosf(2 * div_theta * j);
        const float z = r * sinf(2 * div_theta * j);
        this->vertex.push_back({x, y, z});
        this->uv.push_back({
            1 - sinf(div_theta * j / 2.2f),
            1 - cosf(div_theta * i / 2.f),
        });
        // normal vec is same to vertex vec
        auto v = glm::normalize(this->quat * glm::vec3(x, y, z));
        this->normal.push_back({v.x, v.y, v.z});
      }
      if (i != 0 && i != SPHERE_DIV) {
        points[i - 1].push_back(this->vertex.size() - 1);
      }
    }
  }

  using Face = std::array<GLuint, 3>;
  std::vector<Face> faces;
  for (auto i = 0; i < (int)SPHERE_DIV - 1; ++i) {
    for (auto j = 0; j < points[i].size(); ++j) {
      const GLuint begin = points[i][j];
      const GLuint end   = points[i][(j + 1) % (points[i].size())];
      for (int k = -1; k <= 1; k += 2) {
        GLuint point;
        int    index = i + k;
        if (index < 0) {
          point = 0;
        } else if (index >= SPHERE_DIV - 1) {
          point = this->vertex.size() - 1;
        } else {
          point = ((k == -1) ? begin : end) + (SPHERE_DIV * k);
        }
        this->elements.push_back(begin);
        this->elements.push_back(end);
        this->elements.push_back(point);
        faces.push_back({begin, end, point});
      }
    }
  }

  this->load_texture("assets/earth_1024x512.raw", 1024, 512);
  this->set_buffer_data();
}

void Sphere::animate() {
  this->quat *= glm::quat(glm::vec3(0.f, -0.02f, 0.f));
  this->normal.clear();
  for (auto i : this->vertex) {
    auto v = glm::normalize(this->quat * glm::vec3(i[0], i[1], i[2]));
    this->normal.push_back({v.x, v.y, v.z});
  }
  this->set_buffer_data();
}
}  // namespace gl_learn
