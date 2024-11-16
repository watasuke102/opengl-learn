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
    float rect_size = 0.5f;
    pos[0].x        = rect_size;
    pos[0].y        = rect_size;
    pos[1].x        = rect_size;
    pos[1].y        = -rect_size;
    pos[2].x        = -rect_size;
    pos[2].y        = -rect_size;
    pos[3].x        = -rect_size;
    pos[3].y        = rect_size;
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
  auto bufstat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  E;
  if (bufstat != GL_FRAMEBUFFER_COMPLETE) {
    std::printf("Failed to create FrameBuffer; errno=0x%X\n", bufstat);
    return 1;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

  std::vector<GLubyte> pixels(WIDTH * HEIGHT * 4);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, frame_buffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

  glFlush();

  constexpr uint32_t imagesize = WIDTH * HEIGHT * 3;
  constexpr uint32_t filesize  = imagesize + /* header size = */ 54;
  // clang-format off
  constexpr uint8_t  header[] = {
    // file header
    0x42, 0x4d, // magic
    (filesize & 0x0000'00ff),
    (filesize & 0x0000'ff00) >> 8,
    (filesize & 0x00ff'0000) >> 16,
    (filesize & 0xff00'0000) >> 24,
    0, 0, 0, 0, // reserved
    0x36, 0, 0, 0, // offset to pixel data
    // info header
    0x28, 0, 0, 0, // header size
    WIDTH&0x00ff, (WIDTH&0xff00) >> 8, 0, 0,
    HEIGHT&0x00ff, (HEIGHT&0xff00) >> 8, 0, 0,
    1, 0, // plane number; must be 1
    24, 0, // bits per pixel
    0, 0, 0, 0, // compression = RGB
    (imagesize & 0x0000'00ff),
    (imagesize & 0x0000'ff00) >> 8,
    (imagesize & 0x00ff'0000) >> 16,
    (imagesize & 0xff00'0000) >> 24,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, // neadless info
  };
  // clang-format on
  FILE* f = fopen("texure.bmp", "w+b");
  fwrite(header, sizeof(header), 1, f);
  for (int y = HEIGHT - 1; y >= 0; --y) {
    for (int x = 0; x < (int)WIDTH; ++x) {
      std::fwrite(&pixels[(WIDTH * y + x) * 4 + 2], 1, 1, f); // B
      std::fwrite(&pixels[(WIDTH * y + x) * 4 + 1], 1, 1, f); // G
      std::fwrite(&pixels[(WIDTH * y + x) * 4 + 0], 1, 1, f); // R
    }
    // std::fprintf(f, "\n");
  }
  fclose(f);

  glfwTerminate();
  return 0;
}
