#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>

constexpr uint32_t WIDTH  = 800;
constexpr uint32_t HEIGHT = 600;

int main() {
  if (!glfwInit()) {
    return 1;
  }

  std::cout << "GLFW Initialized" << std::endl;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
      glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Learn", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  std::cout << "Window created: " << window << std::endl;

  glfwMakeContextCurrent(window);
  std::cout << "context created" << std::endl;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
