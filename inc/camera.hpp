#pragma once

#include <GLES3/gl3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "common.hpp"

namespace gl_learn {
class Camera {
 public:
  DISABLE_MOVE_AND_COPY(Camera)
  Camera(float r, float far_clip, float near_clip);

  void view_projection_mat(glm::mat4& mat);
  void animate();

 private:
  glm::vec3 pos;
  float     r;
  float     theta;
  float     far_clip, near_clip;
};
} // namespace gl_learn
