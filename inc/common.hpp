#pragma once

#include <GLES3/gl3.h>

#include <array>
#include <cstdint>

#define DISABLE_MOVE_AND_COPY(Class)       \
  Class(const Class&)            = delete; \
  Class(Class&&)                 = delete; \
  Class& operator=(const Class&) = delete; \
  Class& operator=(Class&&)      = delete;

namespace gl_learn {
using Vertex      = std::array<GLfloat, 3>;
using LineElement = std::array<GLuint, 2>;
using UV          = std::array<GLuint, 2>;

constexpr std::uint32_t WIDTH  = 800;
constexpr std::uint32_t HEIGHT = 600;
}  // namespace gl_learn
