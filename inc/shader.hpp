#pragma once

#include <GLES3/gl3.h>

namespace shader {
GLuint compile_shader(const char* vertex_src, const char* flagment_src);
}
