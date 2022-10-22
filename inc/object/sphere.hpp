#pragma once

#include "common.hpp"
#include "object/object.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace gl_learn {
class Sphere : public Object {
 public:
  DISABLE_MOVE_AND_COPY(Sphere)
  Sphere(glm::vec3 pos, glm::quat quat);
  ~Sphere() = default;
  void init() override;
  void animate() override;
};
} // namespace gl_learn
