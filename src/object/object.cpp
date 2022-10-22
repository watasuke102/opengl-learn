#include "object/object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace gl_learn {
Object::Object() {
  glGenBuffers(1, &this->vertex_buffer);
  glGenBuffers(1, &this->elements_buffer);
}
Object::~Object() {
  glDeleteBuffers(1, &this->vertex_buffer);
  glDeleteBuffers(1, &this->elements_buffer);
}

void Object::set_buffer_data() {
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vertex) * this->vertex.size(),
               this->vertex.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(LineElement) * this->elements.size(),
               this->elements.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::render(GLuint program_id, const glm::mat4& vp_mat) {
  glUseProgram(program_id);

  GLuint    matrix_id = glGetUniformLocation(program_id, "mvp");
  glm::mat4 model_mat =
      glm::translate(glm::mat4(1.f), this->pos) * glm::toMat4(this->quat);
  // std::cout << glm::to_string(model_mat) << "\r";
  glm::mat4 mvp_mat = vp_mat * model_mat;
  glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp_mat[0][0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
  glDrawElements(GL_LINES,
                 sizeof(LineElement) * this->elements.size(),
                 GL_UNSIGNED_INT,
                 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
}
} // namespace gl_learn
