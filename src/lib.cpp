#include "lib.hpp"
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {
std::optional<GLuint> compile(GLuint program_id, int type, const char* src) {
  GLuint shader_id = glCreateShader(type);

  glShaderSource(shader_id, 1, &src, nullptr);
  glCompileShader(shader_id);

  GLint compiled = GL_FALSE;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    int log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    std::vector<char> log_message(log_length);
    glGetShaderInfoLog(shader_id, log_length, NULL, &log_message[0]);
    std::cout << "[Shader error]" << std::endl
              << std::string(log_message.begin(), log_message.end())
              << std::endl;

    glDeleteShader(shader_id);
    glDeleteProgram(program_id);
    return std::nullopt;
  }

  glAttachShader(program_id, shader_id);
  glDeleteShader(shader_id);
  return program_id;
}
} // namespace
namespace shader {

GLuint compile_shader(const char* vertex_src, const char* flagment_src) {
  GLuint                program_id   = glCreateProgram();
  GLint                 link_success = GL_FALSE;
  std::optional<GLuint> shader_id;

  shader_id = compile(program_id, GL_VERTEX_SHADER, vertex_src);
  if (!shader_id) {
    std::cout << "Failed to compile vertex shader" << std::endl;
    goto err;
  }

  shader_id = compile(program_id, GL_FRAGMENT_SHADER, flagment_src);
  if (!shader_id) {
    std::cout << "Failed to compile flagment shader" << std::endl;
    goto err;
  }

  glLinkProgram(program_id);
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_success);
  if (!link_success) {
    std::cout << "Failed to link shader" << std::endl;
    goto err;
  }

  return program_id;

err:
  glDeleteProgram(program_id);
  return 0;
}
} // namespace shader

#define GLSL(s) (const char*)"#version 310 es\n" #s

const char* vertex_shader = GLSL(

    layout(location = 0) in vec3 position;

    out mediump vec3 pos;

    void main() {
      gl_Position = vec4(position, 1.0);
      pos         = gl_Position.xyz;
    }

);
const char* flagment_shader = GLSL(

    in mediump vec3 pos; out mediump vec4 color;

    void main() {
      mediump vec3 threshold = vec3(0);
      mediump vec3 filtered  = step(threshold, pos);
      color                  = vec4(filtered, 1.0);
    }

);

struct Pos {
  GLfloat x, y, z;
};

struct Edge {
  // contain index
  GLuint begin, end;
};

namespace {
GLuint program_id;
GLuint pos_buffer, edge_buffer;
} // namespace

extern "C" {
void init() {
  program_id = shader::compile_shader(vertex_shader, flagment_shader);

  glGenBuffers(1, &pos_buffer);
  glGenBuffers(1, &edge_buffer);

  {
    GLsizeiptr size = sizeof(Pos) * 4;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    Pos* pos =
        (Pos*)glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    float z  = 1.f;
    pos[0].x = 0.5f;
    pos[0].y = 0.5f;
    pos[0].z = z;
    pos[1].x = 0.5f;
    pos[1].y = -0.5f;
    pos[1].z = z;
    pos[2].x = -0.5f;
    pos[2].y = -0.5f;
    pos[2].z = z;
    pos[3].x = -0.5f;
    pos[3].y = 0.5f;
    pos[3].z = z;
    // needless?
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  {
    GLsizeiptr size = sizeof(Edge) * 3;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    Edge* edge = (Edge*)glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    edge[0].begin = 1;
    edge[0].end   = 2;
    edge[1].begin = 2;
    edge[1].end   = 3;
    edge[2].begin = 3;
    edge[2].end   = 0;
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

uint64_t i;
int      draw() {
  glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program_id);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
  glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glFlush();
  return i++;
}
}
