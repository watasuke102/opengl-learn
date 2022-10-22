#pragma once

#include "common.hpp"
#include "object/object.hpp"

namespace gl_learn {
class Plane : public Object {
 public:
  DISABLE_MOVE_AND_COPY(Plane)
  Plane()  = default;
  ~Plane() = default;
  void init() override;
};
} // namespace gl_learn
