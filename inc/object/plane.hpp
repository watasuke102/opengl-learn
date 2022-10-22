#pragma once

#include "common.hpp"
#include "object/object.hpp"

namespace gl_learn {
class Plane : public Object {
 public:
  DISABLE_MOVE_AND_COPY(Plane)
  Plane(float x, float y, float z);
  ~Plane() = default;
  void init() override;
};
} // namespace gl_learn
