#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>

#include "camera.hpp"
#include "shader.hpp"

#define GLSL(s) (const char*)"#version 310 es\n" #s

const char* vertex_shader = GLSL(

    uniform mat4 mvp;

    layout(location = 0) in vec3 position;

    void main() { gl_Position = mvp * vec4(position, 1.0); }

);
const char* flagment_shader = GLSL(

    out mediump vec4 color; void main() { color = vec4(1.0, 0.0, 0.0, 1.0); }

);

struct Pos {
  GLfloat x, y, z;
};

struct Edge {
  // contain index
  GLuint begin, end;
};

int main() {
  if (!glfwInit()) {
    return 1;
  }

  std::cout << "GLFW Initialized" << std::endl;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(
      gl_learn::WIDTH, gl_learn::HEIGHT, "OpenGL Learn", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  std::cout << "Window created: " << window << std::endl;

  glfwMakeContextCurrent(window);
  std::cout << "context created" << std::endl;

  GLuint program_id = shader::compile_shader(vertex_shader, flagment_shader);

  GLuint pos_buffer, edge_buffer;
  glGenBuffers(1, &pos_buffer);
  glGenBuffers(1, &edge_buffer);

  {
    GLsizeiptr size = sizeof(Pos) * 4;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    Pos* pos =
        (Pos*)glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    pos[0].x = 0.5f;
    pos[0].y = 0.5f;
    pos[0].z = 0.1f;
    pos[1].x = 0.5f;
    pos[1].y = -0.5f;
    pos[1].z = 1.f;
    pos[2].x = -0.5f;
    pos[2].y = -0.5f;
    pos[2].z = 1.f;
    pos[3].x = -0.5f;
    pos[3].y = 0.5f;
    pos[3].z = 0.1f;
    // needless?
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  {
    GLsizeiptr size = sizeof(Edge) * 4;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    Edge* edge = (Edge*)glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    edge[0].begin = 0;
    edge[0].end   = 1;
    edge[1].begin = 1;
    edge[1].end   = 2;
    edge[2].begin = 2;
    edge[2].end   = 3;
    edge[3].begin = 3;
    edge[3].end   = 0;
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  gl_learn::Camera camera(3.f, 100.f, 0.01f);
  GLuint           matrix_id = glGetUniformLocation(program_id, "mvp");
  glm::mat4        vp_mat, mvp_mat;

  while (!glfwWindowShouldClose(window)) {
    glViewport(0, 0, gl_learn::WIDTH, gl_learn::HEIGHT);
    glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program_id);

    camera.view_projection_mat(vp_mat);
    // mvp = vp * model
    mvp_mat = vp_mat * glm::mat4(1.f);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp_mat[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
    glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glFlush();

    glfwSwapBuffers(window);
    glfwPollEvents();

    camera.animate();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  glfwTerminate();
  return 0;
}
