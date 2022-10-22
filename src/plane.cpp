#include "plane.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace gl_learn {
Plane::Plane() : pos(0.f, 1.f, 0.f) {
  glGenBuffers(1, &this->vertex_buffer);
  glGenBuffers(1, &this->elements_buffer);

  this->init();
}
void Plane::init() {
  this->vertex.clear();
  this->vertex.assign({
      {0.5f,  0.5f,  0.1f},
      {0.5f,  -0.5f, 1.f },
      {-0.5f, -0.5f, 1.f },
      {-0.5f, 0.5f,  0.1f},
  });
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vertex) * this->vertex.size(),
               this->vertex.data(),
               GL_STATIC_DRAW);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  this->elements.clear();
  this->elements.assign({
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  });
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(Element) * this->elements.size(),
               this->elements.data(),
               GL_STATIC_DRAW);
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Plane::render(GLuint program_id, const glm::mat4& vp_mat) {
  glUseProgram(program_id);

  GLuint    matrix_id = glGetUniformLocation(program_id, "mvp");
  glm::mat4 model_mat = glm::translate(glm::mat4(1.f), this->pos);
  std::cout << glm::to_string(model_mat) << "\r";
  glm::mat4 mvp_mat = vp_mat * model_mat;
  glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp_mat[0][0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
  glDrawElements(
      GL_LINES, sizeof(Element) * this->elements.size(), GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
}
} // namespace gl_learn
