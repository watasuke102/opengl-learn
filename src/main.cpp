#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>

#include "camera.hpp"
#include "object/frame_sphere.hpp"
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
  layout(location = 2) in vec3 normal;

  uniform mat4 mvp;
  out mediump vec2 uv;
  out mediump vec3 I;

  void main() {
    gl_Position = mvp * vec4(position, 1.0);
    uv = vertex_uv;

    // material constant
    const mediump vec3 K_amb    = vec3(0.5);
    const mediump vec3 K_diff   = vec3(1.3);
    const mediump vec3 K_spec   = vec3(0.2);
    const mediump float K_shine = 180.0;

    // Light
    const mediump vec3 L_amb    = vec3(0.0);
    const mediump vec3 L_diff   = vec3(1.0);
    const mediump vec3 L_spec   = vec3(1.0);
    const mediump vec3 L_pos    = vec3(0.0, 0.0, 2.0);

    // vector
    mediump vec3 V = normalize(-gl_Position.xyz);
    mediump vec3 N = normalize(normal);
    mediump vec3 L = normalize(L_pos.xyz - gl_Position.xyz);
    mediump vec3 R = 2.0 * dot(N, L) * N - L;

    mediump vec3 I_amb  = K_amb  * L_amb;
    mediump vec3 I_diff = K_diff * L_diff * max(dot(N, L), 0.0);
    mediump vec3 I_spec = K_spec * L_spec * pow(max(dot(reflect(-L, N), V), 0.0), K_shine);
    I = I_amb + I_diff + I_spec;
  }
);
const char* texture_flagment_shader = GLSL(
  uniform sampler2D texture;
  in  mediump vec2 uv;
  in  mediump vec3 I;
  out mediump vec4 color;

  void main() {
    color = texture2D(texture, uv) + vec4(I.x, I.y, I.z, 0.0);
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLuint normal_program =
      shader::compile_shader(vertex_shader, flagment_shader);
  GLuint texture_program =
      shader::compile_shader(texture_vertex_shader, texture_flagment_shader);

  gl_learn::Camera      camera(3.f, 100.f, 0.01f);
  gl_learn::Sphere      sphere({-0.3f, 0.f, 0.f}, glm::vec3(0.f, 0.f, -37.f));
  gl_learn::FrameSphere frame_sphere(
      {0.3f, 0.f, 0.f}, glm::vec3(0.f, 0.f, 0.f)
  );

  std::cout << "loop start" << std::endl;
  while (!glfwWindowShouldClose(window)) {
    glViewport(0, 0, gl_learn::WIDTH, gl_learn::HEIGHT);
    glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 vp_mat;
    camera.view_projection_mat(vp_mat);

    // frame_sphere.render(normal_program, texture_program, vp_mat);
    sphere.render(normal_program, texture_program, vp_mat);
    glFlush();
    glfwSwapBuffers(window);

    sphere.animate();
    frame_sphere.animate();
    // camera.animate();
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  glfwTerminate();
  return 0;
}
