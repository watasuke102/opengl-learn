#include "object/sphere.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

namespace {
constexpr GLfloat  SPHERE_RADIUS = 1.f;
constexpr uint32_t SPHERE_DIV    = 32;
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

  for (std::uint32_t i = 0; i < (std::uint32_t)this->vertex.size(); ++i) {
    glm::vec3 normal_vec(0.f);
    for (auto e : faces) {
      if (e[0] == i || e[1] == i || e[2] == 0) {
        const auto vert0 = glm::vec3(
            this->vertex[e[0]][0], this->vertex[e[0]][1], this->vertex[e[0]][2]
        );
        const auto vert1 = glm::vec3(
            this->vertex[e[1]][0], this->vertex[e[1]][1], this->vertex[e[1]][2]
        );
        const auto vert2 = glm::vec3(
            this->vertex[e[2]][0], this->vertex[e[2]][1], this->vertex[e[2]][2]
        );
        const auto edge0 = vert1 - vert0;
        const auto edge1 = vert2 - vert0;
        normal_vec += glm::normalize(glm::cross(edge0, edge1));
      }
    }
    normal_vec = glm::normalize(normal_vec);
    this->normal.push_back({normal_vec.x, normal_vec.y, normal_vec.z});
  }

  this->load_texture("assets/earth_1024x512.raw", 1024, 512);
  this->set_buffer_data();
}

void Sphere::animate() {
  this->quat *= glm::quat(glm::vec3(0.f, -0.01f, 0.f));
}
}  // namespace gl_learn
