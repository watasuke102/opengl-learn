#pragma once

#define DISABLE_MOVE_AND_COPY(Class)                                           \
  Class(const Class&)            = delete;                                     \
  Class(Class&&)                 = delete;                                     \
  Class& operator=(const Class&) = delete;                                     \
  Class& operator=(Class&&)      = delete;

namespace gl_learn {
constexpr uint32_t WIDTH  = 800;
constexpr uint32_t HEIGHT = 600;
} // namespace gl_learn
