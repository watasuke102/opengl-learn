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
using Vertex = std::array<GLfloat, 3>;
using UV     = std::array<GLfloat, 2>;
using Normal = std::array<GLfloat, 3>;

constexpr std::uint32_t WIDTH  = 800;
constexpr std::uint32_t HEIGHT = 600;
}  // namespace gl_learn
