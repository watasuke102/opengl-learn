#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>

#include "camera.hpp"
#include "object/plane.hpp"
#include "object/sphere.hpp"
#include "shader.hpp"

#define GLSL(s) (const char*)"#version 310 es\n" #s

// clang-format off
const char* vertex_shader = GLSL(
  layout(location = 0) in vec3 position;

  uniform mat4 mvp;

  void main() {
    gl_Position = mvp * vec4(position, 1.0);
  }
);
const char* flagment_shader = GLSL(
  out mediump vec4 color;

  void main() {
    color = vec4(1.0, 0.0, 0.0, 1.0);
  }
);

const char* texture_vertex_shader = GLSL(
  layout(location = 0) in vec3 position;
  layout(location = 1) in vec2 vertex_uv;

  uniform mat4 mvp;
  out vec2 uv;

  void main() {
    gl_Position = mvp * vec4(position, 1.0);
    uv = vertex_uv;
  }
);
const char* texture_flagment_shader = GLSL(
  uniform sampler2D texture;
  in  mediump vec2 uv;
  out mediump vec4 color;

  void main() {
    color = texture2D(texture, uv);
  }
);
// clang-format on

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
      gl_learn::WIDTH, gl_learn::HEIGHT, "OpenGL Learn", nullptr, nullptr
  );
  if (!window) {
    glfwTerminate();
    return 1;
  }
  std::cout << "Window created: " << window << std::endl;

  glfwMakeContextCurrent(window);
  std::cout << "context created" << std::endl;

  GLuint normal_program =
      shader::compile_shader(vertex_shader, flagment_shader);
  GLuint texture_program =
      shader::compile_shader(texture_vertex_shader, texture_flagment_shader);

  gl_learn::Camera camera(3.f, 100.f, 0.01f);
  // gl_learn::Plane  plane({-0.5f, 0.f, 0.5f}, glm::vec3(0.f, 0.f, 0.f));
  gl_learn::Sphere sphere({0.4f, 0.f, 0.f}, glm::vec3(0.f, 0.f, 10.f));

  std::cout << "loop start" << std::endl;
  while (!glfwWindowShouldClose(window)) {
    glViewport(0, 0, gl_learn::WIDTH, gl_learn::HEIGHT);
    glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 vp_mat;
    camera.view_projection_mat(vp_mat);

    sphere.render(normal_program, texture_program, vp_mat);
    glFlush();
    glfwSwapBuffers(window);

    // sphere.animate();
    // plane.animate();
    camera.animate();
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  glfwTerminate();
  return 0;
}
