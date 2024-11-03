#include "graphic.hpp"
#include "shader.hpp"
#include <GLES3/gl3.h>
#include <IUnityInterface.h>
#include <cstddef>

namespace {
#define GLSL(s) "#version 310 es\n" #s

constexpr char vertex_shader[] = GLSL(

    layout(location = 0) in vec3 position;

    uniform mat4 mvp; out mediump vec3 pos;

    void main() {
      gl_Position = mvp * vec4(position, 1.0);
      pos         = gl_Position.xyz;
    }

);
constexpr char flagment_shader[] = GLSL(

    in mediump vec3 pos; out mediump vec4 color;

    void main() {
      mediump vec3 threshold = vec3(0);
      mediump vec3 filtered  = step(threshold, pos);
      color                  = vec4(filtered, 1.0);
    }

);

GLuint program_id;
GLuint vert_buffer, elem_buffer;
float  mvp_mat[16];

constexpr size_t vert_len = 4;
constexpr size_t elem_len = 3;

unsigned int cnt;
void         draw(int _) {
  ++cnt;
  // glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
  glClearColor(0.f, 0.5f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glUseProgram(program_id);

  GLuint matrix_id = glGetUniformLocation(program_id, "mvp");
  glUniformMatrix4fv(matrix_id, 1, GL_FALSE, mvp_mat);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buffer);
  glDrawElements(GL_TRIANGLE_FAN, elem_len * 2, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glFlush();
}
} // namespace

struct Pos {
  GLfloat x, y, z;
};

struct Edge {
  // contain index
  GLuint begin, end;
};

extern "C" {
void init() {
  program_id = compile_shader(vertex_shader, flagment_shader);

  glGenBuffers(1, &vert_buffer);
  glGenBuffers(1, &elem_buffer);

  {
    GLsizeiptr size = sizeof(Pos) * vert_len;
    glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    Pos* pos =
        (Pos*)glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    float z  = 0.f;
    pos[0].x = 0.5f;
    pos[0].y = 0.5f;
    pos[0].z = z;
    pos[1].x = 0.5f;
    pos[1].y = -0.5f;
    pos[1].z = z;
    pos[2].x = -0.5f;
    pos[2].y = -0.5f;
    pos[2].z = z;
    pos[3].x = -0.5f;
    pos[3].y = 0.5f;
    pos[3].z = z;
    // needless?
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  {
    GLsizeiptr size = sizeof(Edge) * elem_len;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    Edge* edge = (Edge*)glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT);
    edge[0].begin = 1;
    edge[0].end   = 2;
    edge[1].begin = 2;
    edge[1].end   = 3;
    edge[2].begin = 3;
    edge[2].end   = 0;
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

void set_mvp(float x0, float y0, float z0, float w0, //
             float x1, float y1, float z1, float w1, //
             float x2, float y2, float z2, float w2, //
             float x3, float y3, float z3, float w3  //
) {
  mvp_mat[0]  = x0;
  mvp_mat[1]  = y0;
  mvp_mat[2]  = z0;
  mvp_mat[3]  = w0;
  mvp_mat[4]  = x1;
  mvp_mat[5]  = y1;
  mvp_mat[6]  = z1;
  mvp_mat[7]  = w1;
  mvp_mat[8]  = x2;
  mvp_mat[9]  = y2;
  mvp_mat[10] = z2;
  mvp_mat[11] = w2;
  mvp_mat[12] = x3;
  mvp_mat[13] = y3;
  mvp_mat[14] = z3;
  mvp_mat[15] = w3;
}
DrawFnPtr get_draw_fn_ptr() {
  return draw;
}
unsigned int get_cnt() {
  return cnt;
}
}
