#include "camera.hpp"
#include <iostream>

namespace gl_learn {
Camera::Camera(float r, float far_clip, float near_clip)
    : pos(0.f, 0.f, r), r(r), theta(-glm::pi<float>() / 2), far_clip(far_clip),
      near_clip(near_clip) {}

void Camera::view_projection_mat(glm::mat4& mat) {
  glm::mat4 view_mat =
      glm::lookAt(this->pos, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
  glm::mat4 proj_mat = glm::perspective(glm::pi<float>() / 3,
                                        (float)WIDTH / HEIGHT,
                                        this->near_clip,
                                        this->far_clip);
  mat                = proj_mat * view_mat;
}

void Camera::animate() {
  this->pos.x = this->r * cosf(this->theta);
  this->pos.z = this->r * sinf(this->theta);
  this->theta += glm::pi<float>() / 256;
  // std::cout << this->pos.x << ", " << this->pos.z << std::endl;
}
} // namespace gl_learn
