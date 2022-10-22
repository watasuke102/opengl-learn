#pragma once

#include "common.hpp"
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace gl_learn {
class Plane {
 public:
  DISABLE_MOVE_AND_COPY(Plane)
  Plane();
  void init();
  void render(GLuint program_id, const glm::mat4& vp_mat);

 private:
  glm::vec3            pos;
  std::vector<Vertex>  vertex;
  std::vector<Element> elements;
  GLuint               vertex_buffer;
  GLuint               elements_buffer;
};
} // namespace gl_learn
