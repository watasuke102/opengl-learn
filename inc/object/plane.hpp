#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "common.hpp"
#include "object/object.hpp"

namespace gl_learn {
class Plane : public Object {
 public:
  DISABLE_MOVE_AND_COPY(Plane)
  Plane(glm::vec3 pos, glm::quat quat);
  ~Plane() = default;
  void init() override;
  void animate() override;
};
}  // namespace gl_learn
