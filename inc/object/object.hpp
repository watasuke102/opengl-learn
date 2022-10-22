#pragma once

#include "common.hpp"
#include <GLES3/gl3.h>
#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace gl_learn {
class Object {
 public:
  DISABLE_MOVE_AND_COPY(Object)
  Object();
  virtual ~Object();
  virtual void init() = 0;
  void         render(GLuint program_id, const glm::mat4& vp_mat);

 protected:
  glm::vec3            pos;
  std::vector<Vertex>  vertex;
  std::vector<Element> elements;
  void                 set_buffer_data();

 private:
  GLuint vertex_buffer;
  GLuint elements_buffer;
};
} // namespace gl_learn
