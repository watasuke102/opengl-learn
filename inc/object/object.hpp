#pragma once

#include <GLES3/gl3.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

#include "common.hpp"

namespace gl_learn {
class Object {
 public:
  DISABLE_MOVE_AND_COPY(Object)
  Object();
  virtual ~Object();
  virtual void init()    = 0;
  virtual void animate() = 0;

  void render(
      GLuint normal_program, GLuint texture_program, const glm::mat4& vp_mat
  );
  void load_texture(std::string file_name, GLsizei width, GLsizei height);

 protected:
  glm::vec3                pos;
  glm::quat                quat;
  std::vector<Vertex>      vertex;
  std::vector<LineElement> elements;
  std::vector<UV>          uv;
  void                     set_buffer_data();

 private:
  GLuint vertex_buffer;
  GLuint elements_buffer;
  GLuint uv_buffer;
  GLuint texture;
};
}  // namespace gl_learn
