#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "common.hpp"
#include "object/object.hpp"

namespace gl_learn {
class FrameSphere : public Object {
 public:
  DISABLE_MOVE_AND_COPY(FrameSphere)
  FrameSphere(glm::vec3 pos, glm::quat quat);
  ~FrameSphere() = default;
  void init() override;
  void animate() override;
};
}  // namespace gl_learn
