#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>

#include "camera.hpp"
#include "object/sphere.hpp"
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

  gl_learn::Camera camera(3.f, 100.f, 0.01f);
  gl_learn::Sphere sphere({0.4f, 0.f, 0.f}, glm::vec3(0.f, 0.f, 10.f));

  std::cout << "loop start" << std::endl;
  while (!glfwWindowShouldClose(window)) {
    glViewport(0, 0, gl_learn::WIDTH, gl_learn::HEIGHT);
    glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 vp_mat;
    camera.view_projection_mat(vp_mat);

    sphere.render(program_id, vp_mat);
    glFlush();
    glfwSwapBuffers(window);

    // sphere.animate();
    camera.animate();
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  glfwTerminate();
  return 0;
}
