project(
  'opengl-learn', 'cpp',
  default_options : ['warning_level=3', 'cpp_std=c++20']
)

gl_dep = dependency('gl')
glm_dep = dependency('glm')
glfw_dep = dependency('glfw3')

executable (
  meson.project_name(),
  [
    'src/camera.cpp',
    'src/main.cpp',
    'src/shader.cpp',
    'src/object/object.cpp',
    'src/object/plane.cpp',
  ],
  include_directories: include_directories('inc'),
  dependencies: [
    gl_dep,
    glm_dep,
    glfw_dep,
  ]
)
