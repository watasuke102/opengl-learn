#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "shader.hpp"

#define GLSL(s) (const char*)"#version 310 es\n" #s

constexpr uint32_t WIDTH  = 800;
constexpr uint32_t HEIGHT = 600;

const char* vertex_shader = GLSL(

    layout(location = 0) in vec2 position;

    void main() { gl_Position = vec4(position, 0.0, 1.0); }

);
const char* flagment_shader = GLSL(

    out mediump vec4 color; void main() { color = vec4(0.0, 1.0, 0.0, 1.0); }

);

struct Pos {
  GLfloat x, y;
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

  GLFWwindow* window =
      glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Learn", nullptr, nullptr);
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
    pos[1].x = 0.5f;
    pos[1].y = -0.5f;
    pos[2].x = -0.5f;
    pos[2].y = -0.5f;
    pos[3].x = -0.5f;
    pos[3].y = 0.5f;
    // needless?
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  {
    GLsizeiptr size = sizeof(GLuint) * 4;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    GLuint* edge = (GLuint*)glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    edge[0] = 0;
    edge[1] = 1;
    edge[2] = 2;
    edge[3] = 3;
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  // --- setup frame buffer
  GLenum err;
#define E                                                                      \
  do {                                                                         \
    if ((err = glGetError()) != GL_NO_ERROR) {                                 \
      std::fprintf(stderr, "[FATAL] L%d, errno=%X\n", __LINE__, err);          \
      return 1;                                                                \
    }                                                                          \
  } while (0)

  GLuint color_buffer;
  glGenTextures(1, &color_buffer);
  E;
  glBindTexture(GL_TEXTURE_2D, color_buffer);
  E;
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               WIDTH,
               HEIGHT,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               0);
  E;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  E;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  E;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  E;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  E;
  glBindTexture(GL_TEXTURE_2D, 0);
  E;

  GLuint depth_buffer;
  glGenRenderbuffers(1, &depth_buffer);
  E;
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  E;
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
  E;
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  E;

  GLuint frame_buffer;
  glGenFramebuffers(1, &frame_buffer);
  E;
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
  E;
  glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);
  E;
  glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
  E;
  // GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  // glDrawBuffers(1, draw_buffers);
  // E;
  auto bufstat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  E;
  if (bufstat != GL_FRAMEBUFFER_COMPLETE) {
    std::printf("Failed to create FrameBuffer; errno=0x%X\n", bufstat);
    return 1;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  std::vector<GLubyte> pixels(WIDTH * HEIGHT * 4);
  while (!glfwWindowShouldClose(window)) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
    glViewport(0, 0, WIDTH, HEIGHT);
    // glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program_id);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frame_buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glBlitFramebuffer(0,
                      0,
                      WIDTH,
                      HEIGHT,
                      0,
                      0,
                      WIDTH,
                      HEIGHT,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glFlush();

    // FILE* f = fopen("texure.txt", "w+");
    // for (uint32_t y = 0; y < HEIGHT; ++y) {
    //   for (uint32_t x = 0; x < WIDTH; ++x) {
    //     std::fprintf(f,
    //                  "(%02x, %02x, %02x, %02x)",
    //                  pixels[WIDTH * y + x * 4 + 0],
    //                  pixels[WIDTH * y + x * 4 + 1],
    //                  pixels[WIDTH * y + x * 4 + 2],
    //                  pixels[WIDTH * y + x * 4 + 3]);
    //   }
    //   std::fprintf(f, "\n");
    // }
    // fclose(f);
    // break;

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  glfwTerminate();
  return 0;
}
