#include "object/object.hpp"

#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace gl_learn {
Object::Object(GLenum mode) : render_mode(mode), texture(0) {
  glGenBuffers(1, &this->vertex_buffer);
  glGenBuffers(1, &this->elements_buffer);
  glGenBuffers(1, &this->uv_buffer);
}
Object::~Object() {
  glDeleteBuffers(1, &this->vertex_buffer);
  glDeleteBuffers(1, &this->elements_buffer);
  glDeleteBuffers(1, &this->uv_buffer);
}

void Object::load_texture(
    std::string file_name, GLsizei width, GLsizei height
) {
  std::vector<char> buffer;
  {
    std::ifstream f(file_name.c_str(), std::ios::binary | std::ios::ate);
    const auto    size = static_cast<size_t>(f.tellg());
    buffer.resize(size);
    f.seekg(f.beg);
    f.read(buffer.data(), size);
  }

  glGenTextures(1, &this->texture);
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
      buffer.data()
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::set_buffer_data() {
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertex.size(),
      this->vertex.data(), GL_STATIC_DRAW
  );
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->elements.size(),
      this->elements.data(), GL_STATIC_DRAW
  );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(UV) * this->uv.size(), this->uv.data(),
      GL_STATIC_DRAW
  );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::render(
    GLuint normal_program, GLuint texture_program, const glm::mat4& vp_mat
) {
  const GLuint program = this->texture ? texture_program : normal_program;

  glUseProgram(program);

  GLuint    matrix_id = glGetUniformLocation(program, "mvp");
  glm::mat4 model_mat =
      glm::translate(glm::mat4(1.f), this->pos) * glm::toMat4(this->quat);
  // std::cout << glm::to_string(model_mat) << "\r";
  glm::mat4 mvp_mat = vp_mat * model_mat;
  glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp_mat[0][0]);

  if (this->texture) {
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
  }

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
  glDrawElements(
      this->render_mode, sizeof(GLuint) * this->elements.size(),
      GL_UNSIGNED_INT, 0
  );

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
}
}  // namespace gl_learn
